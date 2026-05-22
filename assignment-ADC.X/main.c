/* 
 * File:   main.c
 * Author: fyder9
 *
 * Created on May 14, 2026, 10:25 AM
 */

#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "uart.h"
#include "adc.h"

volatile int g_uart_output_hz = 10;
volatile int g_hz_counter = 0; //uart related variables
//TODO:tidy up the code oin this matter



/* ===========================================================================
 * MAIN
 * =========================================================================== */
int main(int argc, char **argv)
{

    // Disable all analog inputs so digital I/O works on all ports 
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    /* LED output pins */
    TRISAbits.TRISA0 = 0;   /* RA0 = LD1 (output) */
    TRISGbits.TRISG9 = 0;   /* RG9 = LD2 (output) */
    LATAbits.LATA0   = 0;   /* LD1 off             */
    LATGbits.LATG9   = 0;   /* LD2 off             */

    //UART1 Init
    UART1_Init(); //complete with remapping and interrupt setup

    // UART is using the second socket of the mikrobus 
    // TIMER1 for the main loop timing to avoid conflicts with UART1 which uses TIMER2 for baud rate generation
    //ADC uses the first socket of the mikrobus, so it should not conflict with UART1 or TIMER1
/*
    while (1) {
        LATAbits.LATA0   = 1;   // Toggle LD1 on
        //Manual samp and manual conv
        ADC_Init_MSamp_MConv();
        uint16_t val0 = ADC_Start_MSamp_MConv();
        char buf[5];
        sprintf(buf, "%u", val0); // convert integer to string
        UART1_SendString(buf); // Send the ADC value over UART1
        ADC_Deinit(); // Deinitialize ADC
        tmr_wait_ms(TIMER1, 10); // wait for 10ms before next sample

        //Manual samp and auto conv
        ADC_Init_MSamp_AConv();
        val0 = ADC_Start_MSamp_AConv();
        sprintf(buf, "%u", val0); // convert integer to string
        UART1_SendString(buf);

        tmr_wait_ms(TIMER1, 1000); // wait for 1 second before next sample
        ADC_Deinit(); // Deinitialize ADC 
        LATAbits.LATA0   = 0;   // Toggle LD1 off whenever we are not sampling or transmitting data
        
        //Scan Mode
        ADC_Init_ScanMode(0x0820); // Initialize scan mode for AN5 and AN11
        uint16_t ir, bat;
        ADC_Start_ScanMode(&ir, &bat); // Start scan mode and get the conversion results
        char buf_ir[5], buf_bat[5];
        sprintf(buf_ir, "%u", val0);// convert integer to string
        sprintf(buf_bat, "%u", val0);// convert integer to string
        UART1_SendString(buf_ir);
        UART1_SendString(buf_bat);
        tmr_wait_ms(TIMER1, 1000); // wait for 1 second before
        ADC_Deinit(); // Deinitialize ADC   
    }
*/
while(1) {
    LATAbits.LATA0 = 1;
    tmr_wait_ms(TIMER1, 500);
    LATAbits.LATA0 = 0;
    tmr_wait_ms(TIMER1, 500);
}
    return 0;
}
