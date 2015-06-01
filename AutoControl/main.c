#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"

#include "initPA1.h"
#include "initButton.h"
#include "initRTC.h"

#include "tm_lib/tm_stm32f4_delay.h"
#include "tm_lib/tm_stm32f4_hd44780.h"

enum Stan {Rozpoczecie,UstawianieGodziny, UstawianieMinuty,UstawianieRoku, UstawianieMiesiaca, UstawianieDnia, ZapisCzasu, Normalny, Obroty };
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
    uint8_t hour, minute;
    uint16_t year;
    uint8_t month, date;
}CarKomputer;

CarKomputer komp;
Ekran ekran;

void initKomputer(CarKomputer * komp){
	komp->year=15;
	komp->aktualnyStan=Rozpoczecie;
}

void show(CarKomputer * komp){
	int i,j;
	char buffer[5];
	char buffer1[10];
	char buffer2[10];
	char buffer3[16];
	char buffer4[10];
	char buffer5[10];
	char buffer6[10];
	char logoBuf[16] = "AutoControl v1";
	sprintf(buffer,"%d",komp->obroty);
	sprintf(buffer1,"%d %d %d",komp->data.RTC_Year+2000, komp->data.RTC_Month,komp->data.RTC_Date);
	sprintf(buffer2,"%02d:%02d",komp->time.RTC_Hours,komp->time.RTC_Minutes);

	TM_HD44780_Clear();

	switch(komp->aktualnyStan){
	case Rozpoczecie:
		for(j=1;j<=strlen(logoBuf);j++){
			strncpy(buffer3,logoBuf,j);
			Delayms(25);
			TM_HD44780_Puts(0,0,buffer3);

		}
		break;
	case UstawianieGodziny:
		sprintf(buffer4,"Godzina: %d",komp->hour);
		TM_HD44780_Puts(0,0,buffer4);
		break;
	case UstawianieMinuty:
		sprintf(buffer5,"Minuty: %d",komp->minute);
		TM_HD44780_Puts(0,0,buffer5);
		break;
	case ZapisCzasu:
		TM_HD44780_Puts(0,0,"Ustawiono czas");
		break;
	case UstawianieRoku:
		sprintf(buffer6,"%d %d %d",komp->year,komp->month,komp->date);
		TM_HD44780_Puts(0,0,buffer6);
		TM_HD44780_Puts(0,1,"Ustaw date");
		break;
	case UstawianieMiesiaca:
		sprintf(buffer6,"%d %d %d",komp->year,komp->month,komp->date);
		TM_HD44780_Puts(0,0,buffer6);
		TM_HD44780_Puts(0,1,"Ustaw date");
		break;
	case UstawianieDnia:
		sprintf(buffer6,"%d %d %d",komp->year,komp->month,komp->date);
		TM_HD44780_Puts(0,0,buffer6);
		TM_HD44780_Puts(0,1,"Ustaw date");
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
    			komp.aktualnyStan = UstawianieGodziny;
    			break;
    		case UstawianieGodziny:
    			komp.aktualnyStan = UstawianieMinuty;
    			break;
    		case UstawianieMinuty:
    			komp.aktualnyStan = UstawianieRoku;
    			break;
    		case UstawianieRoku:
    			komp.aktualnyStan = UstawianieMiesiaca;
    			break;
    		case UstawianieMiesiaca:
    			komp.aktualnyStan = UstawianieDnia;
    			break;
    		case UstawianieDnia:
    			komp.aktualnyStan = ZapisCzasu;
    			break;
    		case ZapisCzasu:
    			komp.aktualnyStan = Normalny;
    			setTime(komp.hour,komp.minute);
    			setDate(komp.year,komp.month,komp.date);
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


void EXTI3_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        /* Do your stuff when PD0 is changed */

    	if(komp.aktualnyStan == UstawianieGodziny){
    		if(komp.hour <24)
    			komp.hour++;
    		else komp.hour = 0;
    	}

    	if(komp.aktualnyStan == UstawianieMinuty){
    		if(komp.minute < 60)
    			komp.minute++;
    		else komp.minute = 0;
    	}

    	if(komp.aktualnyStan == UstawianieRoku){
    		komp.year++;

    	}

    	if(komp.aktualnyStan == UstawianieMiesiaca){
    		if(komp.month < 12)
    			komp.month++;
    		else komp.month = 0;
    	}
    	if(komp.aktualnyStan == UstawianieDnia){
    		if(komp.date < 31)
    			komp.date++;
    		else komp.date = 0;
    	}
    }

    /* Clear interrupt flag */
    EXTI_ClearITPendingBit(EXTI_Line3);

}



int main(void)
{
	SystemInit();
	Configure_PA1();
	initButton();
	initButtonPE3();
	RTC_Configuration();
	initTIM2();
	initKomputer(&komp);

	TM_HD44780_Init(16,2);


	komp.aktualnyStan = Rozpoczecie;
	show(&komp);
	Delayms(500);
	komp.aktualnyStan = UstawianieGodziny;


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
