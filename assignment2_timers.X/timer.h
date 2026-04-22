/*
 * File:   timer.h
 * Author: fyder9
 *
 * Created on March 10, 2026, 2:40 PM
 */

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif
#define TIMER1 1
#define TIMER2 2
    void tmr_setup_period(int timer, int ms); // defining the function to setup the timer
    void tmr_wait_period(int timer);          // defining the function to wait for the timer to finish counting
    void tmr_wait_ms(int timer, int ms);      // defining the function to wait for a specific number of milliseconds
#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
