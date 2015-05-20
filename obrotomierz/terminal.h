#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "stm32f4xx_usart.h"
#include <stdlib.h>


uint8_t usartGetChar(void);
void usartSendChar(uint8_t znak);
void usartPutString(const char * string);
void usartPutInt(int value);
void usartPutFloat(float value);


void termCursorHide();
void termClearScreen();
void termLocate(int y,int x);

#endif
