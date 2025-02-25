#include "makra.h"
#include "uart/uart.h"
#include <util/delay.h>
#define BAUDRATE 38400


int main(void)
{	
	DDRB |= (1 << DDB5) | (1 << DDB6);  
    DDRE |= (1 << DDE3);  
	//just checking :D
	LED1OFF;
	LED2OFF;
	LED3OFF;
	_delay_ms(500);
	LED1ON;
	LED2ON;
	LED3ON;
	_delay_ms(500);
	LED1OFF;
	LED2OFF;
	LED3OFF;
	_delay_ms(500);
	
	int j = 0;
	UART_Initialization(BAUDRATE);
	UART_SendString("Stlac klavesu: ");
	
	while(1)
	{	
		uint8_t receivedCharacter = UART_ReceiveCharacter();
		
		if (receivedCharacter == 49) 
		{
			UART_SendString("Ahoj stlacil si 1\r\n");		
		} 
		
		else if (receivedCharacter == 50) 
		{
			UART_SendString("Ahoj stlacil si 2\r\n");
		} 
		
		else if (receivedCharacter == 51) 
		{
			LED1CHANGE;
			UART_SendString("Zmenil si LED 1\r\n");	
		} 
		
		else if (receivedCharacter == 52) 
		{
			LED3CHANGE;
			UART_SendString("Zmenil si LED 3\r\n");	
		} 
		
		else if (receivedCharacter !=0) 
		{
			UART_SendString("nedefinovane tlacidlo\r\n");
		}
	} 
}
