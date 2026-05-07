#include <p33EP512MU810.h>
#include <xc.h>
#include "timer.h"
// Implementations for timer module go here.
void tmr_wait_ms(int timer, int ms){
    if (timer == TIMER1) {
        T1CONbits.TON = 0; // Stop timer
        TMR1 = 0; // Clear timer register
        T1CONbits.TCKPS = 0b11; // Set prescaler to 256
        long long period = (FCY / 256) * ms / 1000; // Calculate period value
        PR1 = period; // Set period register
        T1CONbits.TON = 1; // Start timer
        while (!IFS0bits.T1IF); // Wait for timer 1 to expire
        IFS0bits.T1IF = 0; // Clear timer 1 interrupt flag

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Clear timer register
        T2CONbits.TCKPS = 0b11; // Set prescaler to 256
        long long period = (FCY / 256) * ms / 1000; // Calculate period value
        PR2 = period; // Set period register
        T2CONbits.TON = 1; // Start timer
        while (!IFS0bits.T2IF); // Wait for timer 2 to expire
        IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
        
    }
}
void tmr_setup_period(int timer, int ms){

    if (timer == TIMER1) {

        T1CONbits.TON = 0; // Stop timer
        TMR1 = 0; // Clear timer register
        T1CONbits.TCKPS = 0b11; // Set prescaler to 256
        long long period = (FCY / 256) * ms / 1000; // Calculate period value
        PR1 = period; // Set period register
        T1CONbits.TON = 1; // Start timer

    } else if (timer == TIMER2) {
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Clear timer register
        T2CONbits.TCKPS = 0b11; // Set prescaler to 256
        long long period = (FCY / 256) * ms / 1000; // Calculate period value
        PR2 = period; // Set period register
        T2CONbits.TON = 1; // Start timer
        
    }
    else if (timer == TIMER4) {
        T4CONbits.TON = 0; // Stop timer
        TMR4 = 0; // Clear timer register
        TMR5 = 0;
        T4CONbits.T32 = 1;       // 32-bit mode (T4+T5)
        T4CONbits.TCKPS = 0b11;  // prescaler 256
        long long period = ((long long)FCY / 256) * 0.5; // 140625 for 500ms
        PR5 = (period >> 16) & 0xFFFF;
        PR4 = period & 0xFFFF;
        IFS1bits.T5IF = 0;  // clear flag (32-bit mode fires T5 interrupt)
        IEC1bits.T5IE = 1;  // enable Timer5 interrupt (fires in 32-bit T4+T5 mode)
        IPC7bits.T5IP = 1;  // priority level 1
        T4CONbits.TON = 1;
        
    }


}
int tmr_wait_period(int timer){


    if (timer == TIMER1) {
        if (IFS0bits.T1IF) {
          
            return 1; // Timer 1 has expired
        }
        while (!IFS0bits.T1IF); // Wait for timer 1 to expire
        IFS0bits.T1IF = 0; // Clear timer 1 interrupt flag

    } else if (timer == TIMER2) {
        if (IFS0bits.T2IF) {
            return 1; // Timer 2 has expired
        }
        while (!IFS0bits.T2IF); // Wait for timer 2 to expire
        IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
        
    }
    return 0; // Timer has not expired yet
}

void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0;      // clear T5 flag
    // Toggle LD2 (RG9) in the interrupt handler
    LATGbits.LATG9 = !LATGbits.LATG9; // Toggle LD2
}
