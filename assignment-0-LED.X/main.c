/*
 * File:   main.c
 * Author: fyder9
 *
 * Created on February 26, 2026, 4:54 PM
 * Refer to the schematics of the board
 * ASSIGNMENT:
 * -Make the LED blink (one of the two or both RA0-LD1 & RG9-LD2)
 * -Read the T2 button; while it's pressed turn on the LD1
 */

#include "xc.h"
#define FCY 40000000UL
#include <libpic30.h>

int main(void)
{
    // disabling all analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
    // enabling the tris for output
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    TRISEbits.TRISE8 = 1;

    while (1)
    {

        if (PORTEbits.RE8 == 0)
        {

            LATAbits.LATA0 = !LATAbits.LATA0;
            // LATGbits.LATG9 = 1; this is the other LED, but I will just use one for this assignment
            while (PORTEbits.RE8 == 0)
            {
            }
        }
    };

    return 0;
}
