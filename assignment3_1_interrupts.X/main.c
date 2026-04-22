/*
 * File:   main.c
 * Author: fyder9
 * The scope is to use interrupts to toggle an LED when a button is pressed, and also toggle another LED every 500 ms using a timer.
 * Created on March 10, 2026, 2:35 PM
 */
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

/*
 *
 */
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt()
{
    IFS1bits.INT1IF = 0; // reset interrupt flag
    tmr_wait_ms(TIMER2, 20); // debounce delay
    LATGbits.LATG9 = !LATGbits.LATG9;
}
int main(int argc, char **argv)
{

    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    TRISEbits.TRISE8 = 1;    // setting RE8 as input for the button
    INTCON2bits.GIE = 1;     // enabling global interrupts
    RPINR0bits.INT1R = 0x58; // setting the interrupt pin to RE8-T2 num 18
    IFS1bits.INT1IF = 0;     // clearing the interrupt flag
    IEC1bits.INT1IE = 1;     // enabling the interrupt

    // interrupt will be triggered when the button is pressed, and the LED will be toggled

    while (1)
    {
        if (tmr_wait_ms(TIMER1, 500))         // wait for 500 ms
            LATAbits.LATA0 = !LATAbits.LATA0; // toggling the LED
    }
    return 0;
}
