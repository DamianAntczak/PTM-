#include "dht11a.h"

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void dhtTim3Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 84000000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 84;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);
}
void dhtGpioOutInit(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}
void dhtGpioInInit(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
}
void dhtDelay(int us){
	TIM3->CNT = 0;
	while ((TIM3->CNT) <= us);
}
void dhtRead(u8 * rh, u8 * temp, u8 * checkSum ){
	u8 tmp,j,i,tab[5] = {0x00,0x00,0x00,0x00,0x00};
	dhtGpioOutInit();
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	dhtDelay(18000);
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
	dhtDelay(40);
	dhtGpioInInit();

	while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7));
	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7));

	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 8; ++j) {
			while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7));
			TIM_SetCounter(TIM3,0);
			while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7));
			tmp = TIM_GetCounter(TIM3);
			if(tmp<30){// trwanie sygna³u <30us-> 0; ok. 70us -> 1;
				tab[i]=tab[i]<<1;
			}
			else{
				tab[i] = tab[i]<<1;
				tab[i] += 1;
			}
		}
	}
	*rh = tab[0];
	*temp = tab[2];
	*checkSum = tab[4];

}
