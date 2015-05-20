#include "terminal.h"

const char C_HOME[] ="\x1b""[H";
const char S_CLEAR[]="\x1b""[2J";
const char C_HIDE[]="\x1b""[?25l";

uint8_t usartGetChar(void)
{
	// czekaj na odebranie danych
       while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET) {}

   	return USART_ReceiveData(USART3);
}

void usartSendChar(uint8_t znak){
	// wyslanie danych
	USART_SendData(USART3,znak);
	// czekaj az dane zostana wyslane
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) 	{}
}

void usartPutString(const char * string){
	uint8_t znak;
	while((znak=*string++)){
		usartSendChar(znak);
	}
}

void usartPutInt(int value){
	char buffer[10];
	sprintf(buffer,"%d",value);
	usartPutString(buffer);
}

void usartPutFloat(float value){
	char buffer[15];
	sprintf(buffer,"%f",value);
	usartPutString(buffer);
}

void termCursorHide(){
	usartPutString(C_HIDE);

}

void termClearScreen(){
	termCursorHide();
	usartPutString(S_CLEAR);
	usartPutString(C_HOME);
}

void termLocate(int y,int x){
	usartSendChar(0x1b);
	usartSendChar('[');
	usartPutInt(y);
	usartSendChar(';');
	usartPutInt(x);
	usartSendChar('H');
}
