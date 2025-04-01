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
#include <avr/interrupt.h>
#include "libs/libprintfuart.h"
#include <stdio.h>
#include <stdbool.h>


/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

#define CONST 2  
#define ODECET  



// F_CPU definovano primo v projektu!!! Debug->Properties->Toolchain->Symbols

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

int vysledek = 10;
unsigned char uch1 = 255;
unsigned char uch2 = 255;
bool btn1_pressed = false;
bool btn2_pressed = false;
bool temperature_ready = false;

//musime vytvorit soubor pro STDOUT
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void board_init();

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

#define PRESCALE_VALUE 1024
#define PRESCALE 5
#define FREQ 2

/************************************************************************/
/* WEIGHT SENSOR DEFINES								                */
/************************************************************************/

#define ADSK_PIN    PINF   // PF3 as clock (ADSK)
#define ADSK_DDR    DDRF
#define ADSK_PORT   PORTF
#define ADSK_BIT    PF3    // Clock signal on PF3

#define ADDO_PIN    PING   // PG5 as data (ADDO)
#define ADDO_DDR    DDRG
#define ADDO_PORT   PORTG
#define ADDO_BIT    PG5    // Data signal on PG5


void Hx711_init(void) {
	ADDO_DDR &= ~(1 << ADDO_BIT);  // Set ADDO as input
	ADDO_PORT |= (1 << ADDO_BIT);  // Enable pull-up (if needed)
	
	ADSK_DDR |= (1 << ADSK_BIT);   // Set ADSK as output
	ADSK_PORT &= ~(1 << ADSK_BIT); // Start ADSK low
}

unsigned long Hx711_read(void) {
	unsigned long Count = 0;
	uint8_t i;

	// Wait until ADDO goes low (indicates data ready)
	while (ADDO_PIN & (1 << ADDO_BIT));

	// Read 24-bit data
	for (i = 0; i < 24; i++) {
		ADSK_PORT |= (1 << ADSK_BIT);  // Set ADSK high
		Count = Count << 1;
		ADSK_PORT &= ~(1 << ADSK_BIT); // Set ADSK low

		if (ADDO_PIN & (1 << ADDO_BIT)) {
			Count++;
		}
	}

	// Send extra clock pulse to set device mode
	ADSK_PORT |= (1 << ADSK_BIT);
	Count ^= 0x800000; // Convert signed 24-bit value
	//Count 
	ADSK_PORT &= ~(1 << ADSK_BIT);

	return Count;
}


void board_init(){
	UART_init(38400); //nastaveni rychlosti UARTu, 38400b/s
	stdout = &uart_str; //presmerovani STDOUT
}

void printMenu() {
	UART_SendStringNewLine("MENU");
	UART_SendStringNewLine("1 ...... HSX_init");
	UART_SendStringNewLine("0 ...... clear");
}

void cleanConsole() {
	for (int i = 0; i < 30; i++) {
        UART_SendStringNewLine("");  // Send an empty string which is just a newline
    }
}



unsigned long read_average(uint8_t times) {
	unsigned long sum = 0;
	for (uint8_t i = 0; i < times; i++) {
		sum += Hx711_read();
		_delay_ms(20);
	}
	return sum / times;
}



int compare_ul(const void *a, const void *b) {
	unsigned long arg1 = *(const unsigned long *)a;
	unsigned long arg2 = *(const unsigned long *)b;
	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

unsigned long read_median(uint8_t times) {
	if (times == 0) return 0;

	unsigned long values[times];  // Pole na merania

	for (uint8_t i = 0; i < times; i++) {
		values[i] = Hx711_read();
		_delay_ms(2);
	}

	qsort(values, times, sizeof(unsigned long), compare_ul);  // Zotriedi

	if (times % 2 == 0) {
		// Párny po?et ? priemer dvoch stredných
		return (values[times / 2 - 1] + values[times / 2]) / 2;
		} else {
		// Nepárny ? stredná hodnota
		return values[times / 2];
	}
}





// >>> PRIDANE: Kalibrácia a meranie
void kalibracia() {
	UART_SendStringNewLine(">>> KALIBRACIA <<<");

	// 1. Kalibrácia nuly
	UART_SendStringNewLine("Zloz vahu. Kalibrujem nulu...");
	_delay_ms(2000);
	unsigned long zero_offset = read_median(1000);
	UART_SendStringNewLine("Nula nastavena.");

	// 2. Kalibrácia pomocou známeho závažia
	UART_SendStringNewLine("Poloz ZNAME zavazie (napr. 100g) a stlac ENTER...");
	while (!(UART_DataAvailable()));  // ?aká na vstup
	UART_GetChar();  // Pre?íta znak (napr. ENTER)
	_delay_ms(2000);

	unsigned long loaded_value = read_median(1000);
	long diff = loaded_value - zero_offset;

	float known_weight = 200.0;  // ? uprav pod?a použitého závažia
	float scale = known_weight / diff;

	UART_SendStringNewLine("Kalibracia dokoncena. Meriam...");

	// 3. Meranie hmotnosti
	while (1) {
		unsigned long value = Hx711_read();
		long diff = value - zero_offset;
		float weight = diff * scale;

		char buf[30];
		sprintf(buf, "Vaha: %.2f g", weight);
		UART_SendStringNewLine(buf);

		_delay_ms(500);
	}
}



int main(void) {
	
    UART_init(38400);  // Initialize UART with 9600 baud

    // Configure PE5 & PE6 as INPUT
    cbi(DDRE, PORTE5); // Button 1 (PE5)
    cbi(DDRE, PORTE6); // Button 2 (PE6)

    // Enable internal pull-ups (if no external resistors)
    sbi(PORTE, PORTE5);
    sbi(PORTE, PORTE6);

    // Configure external interrupts
    sbi(EICRB, ISC51); cbi(EICRB, ISC50); // INT5 -> Falling edge
    sbi(EICRB, ISC61); cbi(EICRB, ISC60); // INT6 -> Falling edge

    // Enable external interrupts
    sbi(EIMSK, INT5);  
    sbi(EIMSK, INT6);  

    sei(); // Enable global interrupts
		
	uint8_t test_sequence[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'U', 'A', 'R', 'T', '\r', '\n', 0 };

	for (uint8_t i = 0; test_sequence[i] != 0; i++) {
		UART_SendChar(test_sequence[i]);  // Send each character
	}
	
    // Set PB6 as output for LED control
	//sbi(DDRB, PORTB6);
	DDRB |= (1 << DDB6) | (1 << DDB5) | (1 << DDB6);  // Set PORTB pins 5 and 6 as output
    DDRE |= (1 << DDE3);  // Set PORTE pin 3 as output
	
	printMenu();
	while (1) {
		char received = UART_GetChar();  // Wait for input
		UART_SendStringNewLine("STLACIL SI:");
		UART_SendChar(received);
		UART_SendChar('\r');
		UART_SendChar('\n');

		switch (received) {
            case '0':
				cleanConsole();
				printMenu();
            break;  // Exit the program or break the outer loop

			case '1':
			/*
				Hx711_init();
				UART_SendStringNewLine("Init done.");
				unsigned long value = 0;
				UART_SendStringNewLine("Reading weight ADC value:");
				while (1) {
					value = Hx711_read();
					
					
					
					_delay_ms(500);
					char temp_str[20];
					
					
					sprintf(temp_str, "Vaha je: %lu", value);  // Use %lu for unsigned long
    
					UART_SendStringNewLine(temp_str);
					*/
			
			/*
					unsigned long zero_offset = 8224200;   // Kalibrovaná nula
					float scale = 0.005;                   // Príklad konverzného faktora – nastavíš pod?a závažia

					while (1) 
					{
						value = Hx711_read();

						long diff = value - zero_offset;   // Rozdiel od nuly
						float weight_grams = diff * scale;

						char temp_str[30];
						sprintf(temp_str, "Vaha: %.2f g", weight_grams);
						UART_SendStringNewLine(temp_str);

						_delay_ms(500);
					}
				}
				*/
				 case '3':
        Hx711_init();
        UART_SendStringNewLine("Init done.");
        unsigned long value = 0;
        UART_SendStringNewLine("Reading weight ADC value:");
        while (1) {
            value = Hx711_read();
            _delay_ms(500);
            char temp_str[20];
            sprintf(temp_str, "Vaha je: %lu", value);  // Use %lu for unsigned long
            UART_SendStringNewLine(temp_str);
        }
        break;

    // >>> PRIDANE: Nova moznost '2' pre kalibraciu
    case '4':
        Hx711_init();
        UART_SendStringNewLine("Init done.");
        kalibracia(); // Zavolá automatickú kalibráciu a meranie
        break;


			break;
            default:
                UART_SendStringNewLine("Stlac nieco ine :D");
				break;
		}
	}
}