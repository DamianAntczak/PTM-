#ifndef _INITRTC_H_
#define _INITRTC_H_

#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"


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
  RTC_DateStructure.RTC_Date = 0x27;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Tuesday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

  //RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0;
  RTC_TimeStructure.RTC_Minutes = 0;
  RTC_TimeStructure.RTC_Seconds = 0;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
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




#endif
