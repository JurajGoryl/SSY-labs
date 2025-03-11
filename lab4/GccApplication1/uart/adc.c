/*
 * adc.c
 *
 * Created: 3/11/2025 12:27:25
 *  Author: Student
*/


#include "adc.h"

void ADC_init (uint8_t prescale, uint8_t ureference)
{
	ADMUX=0;
	ADCSRA=0;
	ADCSRA|=(prescale<<ADPS0);
	ADMUX|=(ureference<<REFS0);
	sbi(ADCSRA,ADEN);
	while(!(ADCSRB & 0x80));
	while(!(ADCSRB & 0x20));
	
}

uint16_t ADC_get(uint16_t change)
{
	uint16_t tmp=0;
	ADMUX &= ~(31<<MUX0);
	ADCSRB &= ~(1<<MUX5);
	ADMUX |= (change<<MUX0);
	ADCSRA|= (1<<ADSC);
	while((tbi(ADCSRA, ADSC))){}
	
	tmp=ADC;
	ADCSRA|=(1<<ADIF);
	return tmp;
	
}

void ADC_stop(void)
{
	cbi(ADCSRA, ADEN);

}