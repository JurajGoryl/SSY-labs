/*
 * uart.h
 *
 * Created: 2/24/2025 12:14:45
 *  Author: Student
 */ 


#ifndef UART_H_
#define UART_H_


void UART_Initialization(uint16_t br);

void UART_SendCharacter(uint8_t data);

void UART_SendString(char* text);

uint8_t UART_ReceiveCharacter( void );



#endif /* UART_H_ */