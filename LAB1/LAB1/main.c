/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>

#include "uloha.h"
/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/
#define konst 2
#define odecet

#define sbi(var, mask)  ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)  ((var) &= (uint8_t)~(1 << mask))
#define tbi(var,mask)	(var & (1 << mask) )
#define xbi(var,mask)	((var)^=(uint8_t)(1 << mask))
// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/
int x = 10;

unsigned char a = 255;
unsigned char b = 255;

int y = 24;

int hodnota = 200;
char retezec[80] = "Hodnota=";
char buffer[10];
int cislo = 42;
int *ptr = &cislo;

FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

void delay_ms(int ms) {
	while (ms--) {
		_delay_ms(1);
	}
}

int main(void)
{ 	
	board_init();
	_delay_ms(500);
	printf("new--------------------------------------------new");
    _delay_ms(500);
	
	
/*	#ifdef odecet
 	x -= konst;
 	#endif
 	printf("Hodnota je : %d\n\r", x);
	 */

/* 	unsigned int c = (unsigned int)a + (unsigned int)b;
 	printf("Soucet c: %u\n\r", c);
*/

/* 	y = y >> 3;
 	y = y - 1;
 	y = y & 0x2;
 	printf("Vysledek je: %d\n\r", y); 
*/

/* 	itoa(hodnota, buffer, 10);
 	strcat(retezec, buffer);
 	printf("string.h a stdlib.h: %s\n\r", retezec);
 */
 
/* 	char retezec2[80];
 	sprintf(retezec2, "Hodnota=%d", hodnota);
 	printf("sprintf: %s\n\r", retezec2);
 	generateField(NORMAL_CASE); 
 	printField();               
 	capsLetters();              
 	printField();               
 	return 0;
*/

/* 	printf("Hodnota cislo: %d\n\r", *ptr);  
 	printf("Adresa cislo: %p\n\r", (void*)ptr); 
*/

/* 	DDRB |= (1 << DDB6);
 	    while (1)
	    {		   
		    PORTB &= ~(1 << PORTB6);  		    
		    _delay_ms(500);  		    
		    PORTB |= (1 << PORTB6);   
		    _delay_ms(500);
	    }
 		return 0;
*/

DDRB |= (1 << DDB5) | (1 << DDB6);
DDRE |= (1 << DDE3);



while (1)
{
	PORTB = 0b00100000 | 0b01000000;
	sbi(PORTB, PB5);
	sbi(PORTB, PB6);
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0b00000000;
	cbi(PORTE, PE3);
	delay_ms(500);

	PORTB = 96;
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0;
	cbi(PORTE, PE3);
	delay_ms(500);

	PORTB = 0x60;
	sbi(PORTE, PE3);
	delay_ms(500);
	PORTB = 0x00;
	cbi(PORTE, PE3);
	delay_ms(500);

	sbi(PORTB, PB5);
	sbi(PORTB, PB6);
	sbi(PORTE, PE3);
	delay_ms(500);
	cbi(PORTB, PB5);
	cbi(PORTB, PB6);
	cbi(PORTE, PE3);
	delay_ms(500);

	xbi(PORTB, PB5);
	xbi(PORTB, PB6);
	xbi(PORTE, PE3);
	delay_ms(500);
}

return 0;


/* 	int i=0;
     while (1) 
     {
 	_delay_ms(10000);
 	i++;
 	printf("Test x = %d \n\r", i);
     }
	 */
}
