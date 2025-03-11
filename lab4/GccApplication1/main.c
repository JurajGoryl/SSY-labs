#include "makra.h"
#include "uart/uart.h"
#include "uart/timer.h"
#include "uart/AT30.h"
#include <stdbool.h>
#include <util/delay.h>

#define BAUDRATE 38400

uint8_t duty = 10;

int main(void)
{	
	
	bool Timer0 = false;
	bool Timer1 = false;
	bool Timer2 = false;
	
	uint8_t res;
	uint16_t light;
	float temp;
	char out_str[30];
	UART_Initialization(BAUDRATE);
	cbi(DDRE, PORTE5);
	sbi(EICRB, ISC51);
	cbi(EICRB, ISC50);
	sbi(EIMSK, INT5);
	sei();
	
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
		UART_SendString("Your input is: ");
		UART_SendCharacter(recv);
		UART_SendCharacter('\r');
		UART_SendCharacter('\n');
		
		
		switch (recv)
		{
			case '1':
				UART_SendString("abcdefghijklmnopqrstuvwxyz\r\n");
			break;
			
			case '2':
				UART_SendString("ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n");
			break;
			
			case '4':
			
				ADC_init(0x04,0x02);
				light = ADC_get(3);
				sprintf(out_str, "light intensity is: %d\r\n", light);
				UART_SendString(out_str);
			
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
			
			case 't':
				i2c_init();
				
				temp = at30_read_temp();
				sprintf(out_str, "%f\r\n", temp);
				UART_SendString("Temperature is: ");
				UART_SendString(out_str);
			break;
			
			default:
			UART_SendString(strcat(recv, " unknown sequence\r\n"));
			break;
		}
	}	 
}
