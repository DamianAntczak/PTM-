#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"

#include "tm_lib/tm_stm32f4_hd44780.h"
#include "tm_lib/tm_stm32f4_delay.h"

#include "misc.h"
#include "terminal.h"
#include "dht11a.h"
#include <stdlib.h>
#include <stdio.h>

  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;

void RTC_Configuration(void)
{
    RTC_InitTypeDef RTC_InitStructure;


  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

    // Reset the backup domain, clear RTC
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

    if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) /* HSE Already running? */
    {
      /* Enable the LSI OSC */
        RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till LSI is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div8);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Calendar Configuration with HSE/8 supposed at 1 MHz */
  RTC_InitStructure.RTC_AsynchPrediv = 125 - 1;
  RTC_InitStructure.RTC_SynchPrediv  = 8000 - 1; /* (1MHz / 125) - 1 = 7999*/
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);

    /* Set the date: Tuesday April 1st 2014 */
  RTC_DateStructure.RTC_Year = 0x15;
  RTC_DateStructure.RTC_Month = RTC_Month_May;
  RTC_DateStructure.RTC_Date = 0x26;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Tuesday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

  //RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 13;
  RTC_TimeStructure.RTC_Minutes = 40;
  RTC_TimeStructure.RTC_Seconds = 0;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
}


void initUSART3(){

	// wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// wlaczenie taktowania wybranego uk³adu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


	// konfiguracja linii Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// konfiguracja linii Rx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	USART_InitTypeDef USART_InitStructure;
	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even, USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);


	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);


}



/* Configure pins to be interrupts */
void Configure_PD0(void) {
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Tell system that you will use PD0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);

    /* PD0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

//Timer2 Prescaler :2624; Preload = 63999; Actual Interrupt Time = 1s


void initTIM2(){

   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

   	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   	TIM_TimeBaseStructure.TIM_Prescaler =  2624;
   	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   	TIM_TimeBaseStructure.TIM_Period = 63999;
   	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
   	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
   	//
   	//TIM_Cmd(TIM2, ENABLE);

}

void setTime(uint8_t hour, uint8_t min){

	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);
}

void setDate(uint16_t year, uint8_t month, uint8_t date){
	RTC_DateStructure.RTC_Year = year;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Date = date;
	RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);
}


uint8_t impuls = 0;

uint8_t nrZbocza = 0;
int czasImpulsu = 0;

/* Set interrupt handlers */
/* Handle PD0 interrupt */
void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
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
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}



float getFrequency(int czasImpulsu){
	float frequency = 64000/czasImpulsu;
	return frequency;
}



int getRPM(float frequency){
	return frequency*60;
}




uint8_t Rh,Temp,ChkSum;

int main(void)
{


	SystemInit();
	initUSART3();
	Configure_PD0();
	initTIM2();
	RTC_Configuration();
	dhtTim5Init();
	TM_HD44780_Init(16, 2);
	//enableTimerInterrupt();

    TM_HD44780_Puts(0,0, "AutoControl v1");

    Delayms(1000);


	setTime(13,40);


	usartPutString("Obrotomierz v1\r\n");
	//dhtDelay(18000 );

	//usart_putString(" Rowerek\n");


	char buffer[5];
	char buffer1[5];
	char buffer2[5];
	char buffer3[5];
	char buffer4[10];
	char buffer5[5];


	RTC_TimeTypeDef time;
	RTC_DateTypeDef data;
	//dhtRead(&Rh,&Temp,&ChkSum);

	int k=0;
    while(1)
    {
    	//czytanie wartosci z poszeczegolnych peryferiow

    	int czestotliwosc = getRPM(getFrequency(czasImpulsu));
    	RTC_GetTime(RTC_Format_BIN,&time);
    	RTC_GetDate(RTC_Format_BIN,&data);
    	sprintf(buffer1,"%02d:%02d",time.RTC_Hours,time.RTC_Minutes);
    	sprintf(buffer3,"%d",Temp);
    	sprintf(buffer2,"%d",Rh);
    	sprintf(buffer4,"%d %d %d",data.RTC_Year+2000, data.RTC_Month,data.RTC_Date);
    	sprintf(buffer5,"%d",czestotliwosc);

    	TM_HD44780_Clear();
    	TM_HD44780_Puts(0,0, buffer1);
    	TM_HD44780_Puts(6,0, buffer4);
    	if(czestotliwosc>0){
    		TM_HD44780_Puts(0,1, buffer5);
    		TM_HD44780_Puts(6,1, "RPM");
    	}
    	else
    		TM_HD44780_Puts(0,1, "Engine stop");

    	//wyswietlanie na terminalu

    	termClearScreen();
    	termLocate(2,5);
    	usartPutInt(czestotliwosc);
    	termLocate(4,3);

    	usartPutString(buffer1);
		termLocate(5,0);
		usartPutString(buffer3);
		termLocate(5,5);
		usartPutString(buffer2);
		termLocate(6,0);
		usartPutString(buffer4);


    	if(k>50000){
    		dhtRead(&Rh,&Temp,&ChkSum);
    		k = 0;
    	}

    	k++;

    }

}
