#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"

#include "initPA1.h"
#include "initButton.h"
#include "initRTC.h"

#include "tm_lib/tm_stm32f4_delay.h"
#include "tm_lib/tm_stm32f4_hd44780.h"

enum Stan {Rozpoczecie, Normalny, Obroty };
uint8_t nrZbocza = 0;
int czasImpulsu = 0;


typedef struct Layer{
	char buffor[2][16];
	int height;
	int x,y;
};

typedef struct{
	char LCDbuffor[2][16];
	struct Layer layer[3];
}Ekran;

void appendLayer(Ekran *e){
	int i;
	for(i=0;i < 3;i++){
		strcpy(e->LCDbuffor[0],e->layer[i].buffor[0]);
		strcpy(e->LCDbuffor[1],e->layer[i].buffor[1]);
	}
}

typedef struct{
	enum Stan aktualnyStan;
	uint16_t obroty;
	int tempeIn, tempOut;
	int humidity;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef data;
}CarKomputer;

CarKomputer komp;
Ekran ekran;

void show(CarKomputer * komp){
	int i;
	char buffer[5];
	char buffer1[10];
	char buffer2[10];
	sprintf(buffer,"%d",komp->obroty);
	sprintf(buffer1,"%d %d %d",komp->data.RTC_Year+2000, komp->data.RTC_Month,komp->data.RTC_Date);
	sprintf(buffer2,"%02d:%02d",komp->time.RTC_Hours,komp->time.RTC_Minutes);

	TM_HD44780_Clear();

	switch(komp->aktualnyStan){
	case Rozpoczecie:
		TM_HD44780_Puts(0,0,"AutoControl v1");
		break;
	case Normalny:
		TM_HD44780_Puts(0,0,buffer2);
		TM_HD44780_Puts(6,0,buffer1);
		break;
	case Obroty:
		TM_HD44780_Puts(0,0,buffer);
		TM_HD44780_Puts(6,0,"RPM");
		for(i= 0; i < komp->obroty/1000 ;i++){
			TM_HD44780_CursorSet(i,1);
			TM_HD44780_Data(0xFF);
		}
		break;
	}


}


int getRPM(int czasImpulsu){
	float frequency = 64000/czasImpulsu;
	return frequency*60;
}





/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI1_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        /* Do your stuff when PD0 is changed */

    	if(nrZbocza==0){
    		TIM2->CNT = 0;
    		TIM_Cmd(TIM2, ENABLE);
    		nrZbocza=1;
    	}else{
    		czasImpulsu= TIM2->CNT;
    		TIM_Cmd(TIM2, DISABLE);
    		nrZbocza=0;
    	}

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Do your stuff when PD0 is changed */

    	switch(komp.aktualnyStan){
    	case Rozpoczecie:
    		komp.aktualnyStan = Normalny;
    		break;
    	case Normalny:
    		komp.aktualnyStan = Obroty;
    		break;
    	case Obroty:
    		komp.aktualnyStan = Normalny;
    		break;
    	}

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}


int main(void)
{
	SystemInit();
	Configure_PA1();
	initButton();
	RTC_Configuration();
	initTIM2();

	TM_HD44780_Init(16,2);


	komp.aktualnyStan = Rozpoczecie;
	show(&komp);
	Delayms(500);
	komp.aktualnyStan = Normalny;

	show(&komp);
    while(1)
    {
    	komp.obroty = getRPM(czasImpulsu);
    	RTC_GetDate(RTC_Format_BIN,&komp.data);
    	RTC_GetTime(RTC_Format_BIN,&komp.time);
    	show(&komp);
    	Delayms(10);
    }
}
