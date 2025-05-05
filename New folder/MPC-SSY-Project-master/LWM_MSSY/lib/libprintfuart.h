/*
 * UART.h
 *
 * Created: 12.2.2018 13:49:42
 *  Author: Krajsa
 */ 
#include <stdio.h>
#ifndef UART_H_
#define UART_H_

#define RESET_COLOR "\x1B[0m"
#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1B[32m"
#define YELLOW_TEXT "\x1B[33m"
#define BLUE_TEXT "\x1B[34m"

void UART_init(uint16_t Baudrate);
void UART_SendChar(uint8_t data);
uint8_t UART_GetChar( void );
uint8_t UART_GetCharNoWait(void);
void UART_SendString(char *text);
void UART_SendStringNewLine(char *text);
void UART_SendStringNewLineColored(char *str, char *color_code);
int printCHAR(char character, FILE *stream);
#endif /* UART_H_ */