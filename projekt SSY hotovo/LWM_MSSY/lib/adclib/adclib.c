#include "adclib.h"
#include "../macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

// Function to initialize the ADC
void ADC_Init(uint8_t prescale, uint8_t uref) {
    // Set ADC multiplexer and control register to zero
    ADMUX = 0;
    ADCSRA = 0;

    // Set prescaler value for ADC (ADPS0 to ADPS5 bits in ADCSRA)
    ADCSRA |= (prescale << ADPS0);

    // Set the reference voltage for ADC (REFS0 and REFS1 bits in ADMUX)
    ADMUX |= (uref << REFS0);

	sbi (ADCSRA, ADEN);
	while (!(ADCSRB & 0x80));
	while (!(ADCSRB & 0x20));}

// Function to get ADC value from a specific channel (polling method)
uint16_t ADC_get(uint8_t chan) {
    uint16_t tmp = 0;

    // Clear the MUX bits (channel selection)
    ADMUX &= ~(31 << MUX0); // Mask to clear the MUX bits
    ADCSRB &= ~(1 << MUX5); // Clear MUX5 for channels 0-31

    // Set the correct channel in ADMUX
    ADMUX |= (chan << MUX0);

    // Start the ADC conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete (polling)
    while (ADCSRA & (1 << ADSC)) {
        // Wait until ADSC becomes 0
    }

    // Retrieve the ADC result (combine lower and higher 8 bits)
    tmp = ADC;

    // Clear ADC Interrupt Flag (ADIF)
    ADCSRA |= (1 << ADIF);

    return tmp;
}

// Function to stop the ADC conversion
void ADC_stop(void) {
    // Disable the ADC (turn off the ADC module)
    ADCSRA &= ~(1 << ADEN);
}
