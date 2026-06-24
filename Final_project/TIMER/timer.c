// Timer utilities for dsPIC33E
#include <p33EP512MU810.h>
#include <xc.h>
#define FCY 72000000UL
#include "timer.h"
volatile uint8_t time_100ms = 0;
volatile uint8_t time_1s = 0;

int tmr_wait_ms(int timer, int ms) {

    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        TMR1 = 0; //
        T1CONbits.TCKPS = 0b11;
        long long period = ((FCY / 256)/1000) * ms ;
        PR1 = period;
        T1CONbits.TON = 1;
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        TMR2 = 0;
        T2CONbits.TCKPS = 0b11;
        
        long long period = ((FCY / 256)/1000) * ms ;
        PR2 = period;
        T2CONbits.TON = 1;
        while (!IFS0bits.T2IF);
        IFS0bits.T2IF = 0;
    }
    return 1;
}

void tmr_setup_period(int timer, int ms) {
    if (timer == TIMER1) {
        T1CONbits.TON = 0;
        TMR1 = 0;
        T1CONbits.TCKPS = 0b11;
        long long period = ((FCY / 256)/1000) * ms ;
        PR1 = period;
        T1CONbits.TON = 1;

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0;
        TMR2 = 0;
        T2CONbits.TCKPS = 0b11;
        long long period = ((FCY / 256)/1000) * ms ;
        PR2 = period;
        // Clear interrupt flag and enable interrupt
        IFS0bits.T2IF = 0;  
        IEC0bits.T2IE = 1;
        T2CONbits.TON = 1;


    } else if (timer == TIMER4) {
        T4CONbits.TON = 0;
        TMR4 = 0;
        TMR5 = 0;
        T4CONbits.T32 = 1;
        T4CONbits.TCKPS = 0b11;
        // ms =100 
        long long period = ((long long)FCY / 256/ 1000) * ms ;
        PR5 = (period >> 16) & 0xFFFF;
        PR4 = period & 0xFFFF;
        IFS1bits.T5IF = 0;
        IEC1bits.T5IE = 1;
        IPC7bits.T5IP = 1;
        T4CONbits.TON = 1;
    }
}

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

// Timer5 ISR: 1 second
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0;
    time_1s++;
}
//timer 2 ISR: 100 ms
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;
    time_100ms++;
    
}