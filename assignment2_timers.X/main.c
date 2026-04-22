/*
 * File:   main.c
 * Author: fyder9
 *
 * Created on March 10, 2026, 2:35 PM
 */

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

    tmr_setup_period(TIMER1, 200); // passing TIMER1 as an argument, was defined in timer.h
    while (1)
    {
        if (tmr_wait_period(TIMER1))          // we wait inside the function!!!
            LATAbits.LATA0 = !LATAbits.LATA0; // toggling the LED
    }
    return 0;
}
