
/*
 * timer.h
 *
 * Created: 3/4/2025 12:39:38
 *  Author: Student
 */ 
#ifndef TIMER_H_
#define TIMER_H_

void Timer1_cmp_start();

void Timer2_fastpwm_start(uint8_t duty);

void Timer0_ovf_start();

void Timer0_konec();
void Timer1_konec();
void Timer2_konec();

#endif /* TIMER_H_ */