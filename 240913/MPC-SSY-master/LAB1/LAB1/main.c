/*
 * LAB1.c
 *
 * Created: 02.02.2020 9:01:38
 * Author : Ondra
 */ 

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include "libs/macros.h"
#include <avr/io.h>
#include <util/delay.h>
#include "libs/libprintfuart.h"
#include <stdio.h>

/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

#define CONST 2  
#define ODECET  
#define UPPER_CASE 1
#define NORMAL_CASE 2
#define DIRECTION_UP 1
#define DIRECTION_DOWN 2


// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

int vysledek = 10;
unsigned char uch1 = 255;
unsigned char uch2 = 255;

//musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

// Globalni pole
char abeceda[26];

// funkce pro generaci abecedy
void generateField(int case_type) {
	int i;

	if (case_type == UPPER_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = 'A' + i;
		}
		} else if (case_type == NORMAL_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = 'a' + i;
		}
		} else {
		// spatna vstupni promenna	
		printf("PROGRAM ERROR\n");
		return;
	}
}

// funkce pro zmenu velikosti pismen
void capsLetters(int case_type) {
	int i;

	if (case_type == UPPER_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = toupper(abeceda[i]); // zmen na velka
		}
		} else if (case_type == NORMAL_CASE) {
		for (i = 0; i < 26; i++) {
			abeceda[i] = tolower(abeceda[i]); // zmen na mala
		}
		} else {
		printf("PROGRAM ERROR\n");
		return;
	}
}

// Funkce pro výpis abecedy v požadovaném sm?ru
void printField(int direction) {
	int i;
	
	if (direction == DIRECTION_UP) {
		for (i = 0; i < 26; i++) {
			printf("%c ", abeceda[i]);
		}
		} else if (direction == DIRECTION_DOWN) {
		for (i = 25; i >= 0; i--) {
			printf("%c ", abeceda[i]);
		}
		} else {
		// spatny smer
		printf("PROGRAM ERROR\n");
	}
}


void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

int main(void)
{ 	
	#ifdef ODECET
	vysledek = vysledek - CONST;
	#endif
	

	
	int int1 = (int)uch1 + (int)uch2;
	
	#include <stdio.h>
	
    int variable2 = 24;

    // posunuti o 3 bity
    variable2 = variable2 >> 3;
	variable2 = variable2 - 1;
    // Vymaskovani
    variable2 = variable2 & 0x2;
		
	char hodnota[80] = "HODNOTA=";
	int cislo = 99;

	char retezec1[120];
	sprintf(retezec1, "%s%d", hodnota, cislo);
	char retezec2[120] = "";
	strcpy(retezec2, hodnota);
	sprintf(retezec2 + strlen(retezec2), "%d", cislo);
		
	generateField(UPPER_CASE);
	capsLetters(NORMAL_CASE);
	capsLetters(UPPER_CASE);
	
	board_init();
	_delay_ms(1000);
	printf("1) vysledek je: %d \n\r", vysledek);
	printf("2) vysledek je: %d \n\r", int1);
	printf("3) vysledek je: %d \n\r", variable2);
	printf("4) vysledek je: %s \n\r", retezec1);
	printf("4) vysledek je: %s \n\r", retezec2);
	printf("\n\r");
	printField(DIRECTION_UP);
	printf("\n\r");
	printf("Hello word\n\r");
    _delay_ms(1000);
	int i=0;
	DDRB |= (1 << DDB5) | (1 << DDB6);  // Set PORTB pins 5 and 6 as output
    DDRE |= (1 << DDE3);  // Set PORTE pin 3 as output
    while (1) 
    {
	LED0ON;	LED1ON;	LED2ON;	_delay_ms(1000);
	i++;
	printf("Test x = %d \n\r", i);
	LED0OFF;	LED1OFF;	LED2OFF;	_delay_ms(1000);	}
}

