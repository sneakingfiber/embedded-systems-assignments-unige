#include "timer.h"
#include "xc.h"
#define FCY 72000000UL // instruction cycle frequency, 72MHz for PIC24FJ128GA010

void tmr_setup_period(int timer, int ms)
// CONFIGURING THE TIMER TO COUNT FOR A SPECIFIC TIME IN MS
{
    if (timer == TIMER1)
    {
        T1CONbits.TON = 0;                                         // stopping the timer
        TMR1 = 0;                                                  // setting timer counter value to 0
        T1CONbits.TCKPS = 0b11;                                    // setting the prescaler binary 10 = 64
        unsigned long tmp = ((FCY * ms) / T1CONbits.TCKPS) / 1000; // setting the time limit for the timer and convert to ms
        PR1 = tmp;                                                 // PR1 is the number that the timer counts to before it resets and sets the interrupt flag
        T1CONbits.TON = 1;
    }
    if (timer == TIMER2)
    {
        T2CONbits.TON = 0;                                         // stopping the timer (TON= timer on bit)
        TMR2 = 0;                                                  // setting timer value to 0
        T2CONbits.TCKPS = 0b11;                                    // setting the prescaler ( FCY * ms)/ prescaler;
        unsigned long tmp = ((FCY * ms) / T2CONbits.TCKPS) / 1000; // setting the time limit for the timer and convert to ms
        PR2 = tmp;                                                 // now counting till 200ms
        T2CONbits.TON = 1;
    }
};

void tmr_wait_period(int timer)
{
    if (timer == TIMER1)
    {
        while (!IFS0bits.T1IF)
        {
            // waiting for the timer to finish counting, the T1IF bit will be set when the timer reaches the value in PR1
        }
        IFS0bits.T1IF = 0;
    }
    if (timer == TIMER2)
    {
        while (!IFS0bits.T2IF)
        {
            // waiting for the timer to finish counting, the T2IF bit will be set when the timer reaches the value in PR2
        }
        IFS0bits.T2IF = 0;
    }
};

int tmr_wait_ms(int timer, int ms)
{
    if (timer == TIMER1)
    {
        T1CONbits.TON = 0; // stopping the timer
        TMR1 = 0;          // setting timer counter value to 0
        if (ms == 0 || ms > 200)
        {                           // the limit is 200ms because with a prescaler of 1:256, the timer can count up to 65535, which corresponds to approximately 233ms at 72MHz{
            ms = 1;                 // minimum time is 1ms
            T1CONbits.TCKPS = 0b00; // setting the prescaler to 1:1
        }
        else if (ms <= 7) // the limit is 7ms because with a prescaler of 1:8, the timer can count up to 65535, which corresponds to approximately 7.3ms at 72MHz
        {
            T1CONbits.TCKPS = 0b01; // setting the prescaler to 1:8
        }
        else if (ms <= 58)
        {
            T1CONbits.TCKPS = 0b10; // setting the prescaler to 1:64
        }
        else
        {                           // case for ms between 58 and 200ms
            T1CONbits.TCKPS = 0b11; // setting the prescaler to 1:256
        }

        unsigned long tmp = ((FCY * ms) / T1CONbits.TCKPS) / 1000; // setting the time limit for the timer and convert to ms
        PR1 = tmp;                                                 // PR1 is the number that the timer counts to before it resets and sets the interrupt flag
        T1CONbits.TON = 1;

        while (!IFS0bits.T1IF)
        {
            // waiting for the timer to finish counting, the T1IF bit will be set when the timer reaches the value in PR1
        }
        IFS0bits.T1IF = 0;
        return 1; // returning 1 to indicate that the timer has finished counting
    }
    if (timer == TIMER2)
    {
        T2CONbits.TON = 0; // stopping the timer
        TMR2 = 0;          // setting timer counter value to 0
        if (ms == 0 || ms > 200)
        {
            ms = 1;
            T2CONbits.TCKPS = 0b00; // setting the prescaler to 1:1
        }
        else if (ms <= 7)
        {
            T2CONbits.TCKPS = 0b01; // setting the prescaler to 1:8
        }
        else if (ms <= 58)
        {
            T2CONbits.TCKPS = 0b10; // setting the prescaler to 1:64
        }
        else
        {
            T2CONbits.TCKPS = 0b11; // setting the prescaler to 1:256
        }

        unsigned long tmp = ((FCY * ms) / T2CONbits.TCKPS) / 1000;
        PR2 = tmp;
        T2CONbits.TON = 1;

        while (!IFS0bits.T2IF)
        {
            // waiting for the timer to finish counting
        }
        IFS0bits.T2IF = 0;
        return 1;
    }
};