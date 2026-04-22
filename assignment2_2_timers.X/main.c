/*
 * File:   main.c
 * Author: fyder9
 *
 * Created on March 10, 2026, 2:35 PM
 */
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

/*
 *
 */

int main(int argc, char **argv)
{

    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
    TRISAbits.TRISA0 = 0;

    while (1)
    {
        if (tmr_wait_ms(TIMER1, 20))          // we wait inside the function!!!
            LATAbits.LATA0 = !LATAbits.LATA0; // toggling the LED

        if (tmr_wait_ms(TIMER1, 200)) // we wait inside the function!!!
            LATAbits.LATA0 = !LATAbits.LATA0;
    }
    return 0;
}
