#include "makra.h"
#include "uart/uart.h"
#include "uart/timer.h"
#include <stdbool.h>
#include <util/delay.h>

#define BAUDRATE 38400

uint8_t duty = 10;

int main(void)
{	
	
	bool Timer0 = false;
	bool Timer1 = false;
	bool Timer2 = false;
	DDRB |= (1 << DDB4) |(1 << DDB5) | (1 << DDB6);  
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
	
	UART_Initialization(BAUDRATE);
		
	UART_SendString("MENU:\r\n");
	UART_SendString("1: MalaAbeceda\r\n");
	UART_SendString("2: VelkaAbeceda\r\n");
	UART_SendString("4: Timer0 ON\r\n");
	UART_SendString("5: Timer1 ON\r\n");
	UART_SendString("+: LED3 pridaj jas \r\n");
	UART_SendString("-: LED3 uber jas \r\n");

	
	while (1) 
	{
		char recv = UART_ReceiveCharacter();
		switch (recv)
		{
			case '1':
				UART_SendString("abcdefghijklmnopqrstuvwxyz\r\n");
			break;
			
			case '2':
				UART_SendString("ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n");
			break;
			
			case '4':
			
			
			break;
			
			case '5':
				if (Timer1 == false) 
				{
					Timer1 = true;
					Timer1_cmp_start();
				}
				 else 
				{
					Timer1 = false;
					Timer1_konec();
				}
			break;
			
			case '+':
				if (duty > 10) 
				{
					duty -= 10;
				}
				Timer2_fastpwm_start(duty);
			break;
			
			case '-':
				if (duty < 100) 
				{
					duty += 10;
				}
			Timer2_fastpwm_start(duty);
			break;
			
			default:
			UART_SendString(strcat(recv, " unknown sequence\r\n"));
			break;
		}
	}
	


	 
}
