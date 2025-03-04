/*
 * timer.c
 *
 * Created: 3/4/2025 12:40:11
 *  Author: Student
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "../makra.h"

#define PRESCALE 5
#define PRESCALE_VALUE 1024
#define  FREQUENCY 2

void Timer0_konec ()
{
	cli();
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	sei();	
}

void Timer1_konec()
{
	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;
	sei();
}

void Timer2_konec()
{
	cli();
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	sei();
}

void Timer1_cmp_start()
{
	uint16_t cmp = (F_CPU / (2*PRESCALE_VALUE*FREQUENCY)) -1;
	cli();
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;
	OCR1A = cmp;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= PRESCALE;
	TIMSK1 |= (1 << OCIE1A);
	TCCR1A |= (1 << COM1A0);
	sei();	
}



void Timer0_ovf_start()
{
	cli();
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;
	TCCR0B |= PRESCALE;
	TCCR0A |= (1 << COM0A0);
	TIMSK0 |= (1 << TOIE0);
	sei();
}

void Timer2_fastpwm_start(uint8_t duty)
{
	cli();
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	uint8_t div = (255*duty)/100;
	OCR2A = div;
	TCCR2A |= (1 << WGM21);
	TCCR2A |= (1 << WGM20);
	TCCR2B |= PRESCALE;
	TIMSK2 |= (1 << TOIE2);
	TCCR2A |= (1 << COM2A1);
	sei();
}

ISR(TIMER1_COMPA_vect) {
	LED1CHANGE;
}

ISR(TIMER2_COMPA_vect) {
	 LED3CHANGE;
}