/* 
 * File:   main.c
 * Author: fyder9
 *
 * Created on May 5, 2026, 7:58 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.h"
/*
 * 
 */

void algorithm(){
    tmr_wait_ms(TIMER2, 7);
}

void readUART1(){
    // setting up UART at 9600 baud rate
    U1BRG = 11;            // (7400000 / 4) / (16 ? 9600) ? 1
    U1MODEbits.UARTEN = 1; // enabling UART
    U1STAbits.UTXEN = 1;   // enabling UART-TX
}

int main(int argc, char** argv) {
    // disabling all analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
    // enabling the tris for output
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    TRISDbits.TRISD11 = 1; // setting RD11 as input for RX
    TRISDbits.TRISD0 = 0;  // setting RD0 as output for TX
    int counter = 0;
    tmr_setup_period(TIMER1, 10);
    
    int counter = 0;
    
    while(1) {
        algorithm();
        // code to handle the assignment
        unsigned char readValue = readUART1;
        counter ++;
        if(counter == 50) {
            LATAbits.LATA0 = !LATAbits.LATA0;
            counter = 0;
        }
    }  
    

    
    return (EXIT_SUCCESS);
}

