#ifndef TIMER_H
#define TIMER_H

// Declarations for timer module go here.
#define TIMER1 1
#define TIMER2 2
#define FCY 4000000UL
void tmr_wait_ms(int timer, int ms); // Waits for the specified timer to reach the specified time in milliseconds. This function blocks until the timer expires.

// Initializes the timer module. Must be called before any other timer functions.
void tmr_setup_period(int timer, int ms);
// Waits for the specified timer to reach its period. This function blocks until the timer expires.
int tmr_wait_period(int timer);


#endif // TIMER_H