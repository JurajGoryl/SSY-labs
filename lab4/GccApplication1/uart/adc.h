/*
 * adc.h
 *
 * Created: 3/11/2025 12:27:36
 *  Author: Student
 */ 

#ifndef ADC_H_
#define ADC_H_
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "../makra.h"

void ADC_init(uint8_t prescale, uint8_t ureference);
uint16_t ADC_get(uint16_t change);
void ADC_stop(void);

