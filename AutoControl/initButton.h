#ifndef _INITBUTTON_H_
#define _INITBUTTON_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"

void initButton()
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitTypeDef GPIO_Init_Str;
	GPIO_Init_Str.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Str.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init_Str.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_Init_Str.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Str);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStruct;
	// PD0 is connected to EXTI_Line0
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}

void initButtonPE3(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);


	GPIO_InitTypeDef GPIO_Init_Str;
	GPIO_Init_Str.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Str.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Str.GPIO_Pin = GPIO_Pin_3 ;
	GPIO_Init_Str.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&GPIO_Init_Str);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

	EXTI_InitTypeDef EXTI_InitStruct;
	// PD0 is connected to EXTI_Line0
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}

void initButtonPC6(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


	GPIO_InitTypeDef GPIO_Init_Str;
	GPIO_Init_Str.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Str.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Str.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_Init_Str.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_Init_Str);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);

	EXTI_InitTypeDef EXTI_InitStruct;
	// PD0 is connected to EXTI_Line0
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void initButtonPC7(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


	GPIO_InitTypeDef GPIO_Init_Str;
	GPIO_Init_Str.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Str.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Str.GPIO_Pin = GPIO_Pin_7 ;
	GPIO_Init_Str.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_Init_Str);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);

	EXTI_InitTypeDef EXTI_InitStruct;
	// PD0 is connected to EXTI_Line0
	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void initButtonPC8(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


	GPIO_InitTypeDef GPIO_Init_Str;
	GPIO_Init_Str.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Str.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init_Str.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_Init_Str.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_Init_Str);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);

	EXTI_InitTypeDef EXTI_InitStruct;
	// PD0 is connected to EXTI_Line0
	EXTI_InitStruct.EXTI_Line = EXTI_Line8;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;

	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}



#endif
