#include <p33EP512MU810.h>
#include <xc.h>
#include "timer.h"

// Blocking wait for specified milliseconds
void tmr_wait_ms(int timer, int ms) {
    if (timer == TIMER1) {
        T1CONbits.TON = 0;                    // stop timer
        TMR1 = 0;                             // clear timer register
        T1CONbits.TCKPS = 0b11;               // prescaler 256
        long long period = (FCY / 256) * ms / 1000; // calculate period
        PR1 = period;                         // set period register
        T1CONbits.TON = 1;                    // start timer
        while (!IFS0bits.T1IF);               // wait for timeout
        IFS0bits.T1IF = 0;                    // clear interrupt flag

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;                    // stop timer
        TMR2 = 0;                             // clear timer register
        T2CONbits.TCKPS = 0b11;               // prescaler 256
        long long period = (FCY / 256) * ms / 1000; // calculate period
        PR2 = period;                         // set period register
        T2CONbits.TON = 1;                    // start timer
        while (!IFS0bits.T2IF);               // wait for timeout
        IFS0bits.T2IF = 0;                    // clear interrupt flag
    }
}

// Setup a periodic timer (non-blocking, requires tmr_wait_period to sync)
void tmr_setup_period(int timer, int ms) {
    if (timer == TIMER1) {
        T1CONbits.TON = 0;                    // stop timer
        TMR1 = 0;                             // clear timer register
        T1CONbits.TCKPS = 0b11;               // prescaler 256
        long long period = (FCY / 256) * ms / 1000; // calculate period
        PR1 = period;                         // set period register
        T1CONbits.TON = 1;                    // start timer

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;                    // stop timer
        TMR2 = 0;                             // clear timer register
        T2CONbits.TCKPS = 0b11;               // prescaler 256
        long long period = (FCY / 256) * ms / 1000; // calculate period
        PR2 = period;                         // set period register
        T2CONbits.TON = 1;                    // start timer

    } else if (timer == TIMER4) {
        T4CONbits.TON = 0;                    // stop timer
        TMR4 = 0;                             // clear timer 4
        TMR5 = 0;                             // clear timer 5
        T4CONbits.T32 = 1;                    // 32-bit mode (T4+T5)
        T4CONbits.TCKPS = 0b11;               // prescaler 256
        long long period = ((long long)FCY / 256) * ms / 1000; // calculate period
        PR5 = (period >> 16) & 0xFFFF;
        PR4 = period & 0xFFFF;
        IFS1bits.T5IF = 0;                    // clear flag (32-bit mode fires T5)
        IEC1bits.T5IE = 1;                    // enable Timer5 interrupt
        IPC7bits.T5IP = 1;                    // priority level 1
        T4CONbits.TON = 1;                    // start timer
    }
}

// Wait for periodic timer to expire
int tmr_wait_period(int timer) {
    if (timer == TIMER1) {
        if (IFS0bits.T1IF) {
            IFS0bits.T1IF = 0;
            return 1;
        }
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;

    } else if (timer == TIMER2) {
        if (IFS0bits.T2IF) {
            IFS0bits.T2IF = 0; 
            return 1;
        }
        while (!IFS0bits.T2IF);
        IFS0bits.T2IF = 0;
    }
    return 0;
}

// Timer5 interrupt for LD2 blinking (500 ms period → 1 Hz toggle)
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0;       // clear interrupt flag
    LATGbits.LATG9 = !LATGbits.LATG9; // toggle LD2 (RG9)
}
