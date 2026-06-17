// Timer module header
#ifndef TIMER_H
#define TIMER_H

#define TIMER1 1
#define TIMER2 2
#define TIMER4 4
#define FCY 72000000UL

void tmr_wait_ms(int timer, int ms);
void tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);

#endif