#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#define F_CPU 8000000 

void UART_Initialization(uint16_t br)
{
	int ubbr = ((F_CPU/16/br)-1);
	UBRR1H = (uint8_t)(ubbr>>8);
	UBRR1L = (uint8_t)ubbr;
	
	UCSR1B = (1<<RXCIE1)|(1<<TXCIE1)|(1<<RXEN1)|(1<<TXEN1);
}


void UART_SendCharacter(uint8_t data)
{
	while ( !( UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
}

void UART_SendString(char*text) {
	
	for (int i=0; i < strlen(text); i++) {
		if (text[i] == 0x00) {
			return;
		}
		UART_SendCharacter(text[i]);
	}
}

uint8_t UART_ReceiveCharacter( void )
{
	while ( !(UCSR1A & (1<<RXC1)) );
	return UDR1;
}

ISR(USART1_RX_vect) {
	uint8_t recv;
	recv = UART_ReceiveCharacter();
	if (recv == 1) {
		UART_SendCharacter('1');
	}
}
