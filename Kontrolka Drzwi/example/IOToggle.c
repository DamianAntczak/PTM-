#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"

int Drzwi_prawe = 0;
int Drzwi_lewe = 0;
int Bagaznik = 0;

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

void IOToggle(void)
{
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure PD0 in output pushpull mode user button pin GPIOA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  			//numer przerwania
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 	//priorytet g³ówny
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; 		//subpriorytet
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//uruchom dany kana³
  NVIC_Init(&NVIC_InitStructure); 								//zapisz wype³nion¹ strukturê do rejestrów

  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = GPIO_Pin_0;  					//wybór numeru aktualnie konfigurowanej linii przerwañ
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  			//wybór trybu - przerwanie b¹dŸ zdarzenie
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//wybór zbocza, na które zareaguje przerwanie
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  					//uruchom dan¹ liniê przerwañ
  EXTI_Init(&EXTI_InitStructure);  								//zapisz strukturê konfiguracyjn¹ przerwañ zewnêtrznych do rejestrów

  SYSCFG_EXTILineConfig(GPIOD, EXTI_PinSource0);

  while (1)
  {

  }

}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
		Delay(100000);
		Drzwi_prawe = 1;
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);

		EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
		Delay(100000);
		Drzwi_lewe = 1;
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);

		EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
		Delay(100000);
		Bagaznik = 1;
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14);

		EXTI_ClearITPendingBit(EXTI_Line2);
    }
}


void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
