#ifndef ADCLIB_H_
#define ADCLIB_H_

#include <stdint.h>

#define ADC_MAX_CHANNELS 8

// ADC reference voltage options
#define ADC_VREF_AREF 0
#define ADC_VREF_AVCC 1
#define ADC_VREF_INTERNAL 3

// Function to initialize the ADC
void ADC_Init(uint8_t prescale, uint8_t uref);

// Function to start an ADC conversion and get the result
uint16_t ADC_get(uint8_t chan);

// Function to stop the ADC
void ADC_stop(void);

#endif // ADCLIB_H
