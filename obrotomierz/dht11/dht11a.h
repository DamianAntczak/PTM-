#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

void dhtRead(u8 * rh, u8 * temp, u8 * checkSum);
void dhtDelay(int us);
void dhtGpioInInit(void);
void dhtGpioOutInit(void);
void dhtTim3Init(void);
