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
#include "led.h"
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

    /* setup LED output pins */
    LED_Init();
    //UART1 Init
    UART1_Init(); //complete with remapping and interrupt setup

    // UART is using the second socket of the mikrobus 
    // TIMER1 for the main loop timing to avoid conflicts with UART1 which uses TIMER2 for baud rate generation
    //ADC uses the first socket of the mikrobus, so it should not conflict with UART1 or TIMER1

    while (1) {
        //Manual samp and manual conv
        ADC_Init_MSamp_MConv();
        uint16_t val0 = ADC_Start_MSamp_MConv();
        char buf[6];
        sprintf(buf, "%u", val0); // convert integer to string
        UART1_SendString("; BAT:");
        UART1_SendString(buf); // Send the ADC value over UART1
        ADC_Deinit(); // Deinitialize ADC
        tmr_wait_ms(TIMER1, 10); // wait for 10ms before next sample
        LED_Blink_Twice();
        //Manual samp and auto conv
        ADC_Init_MSamp_AConv();
        val0 = ADC_Start_MSamp_AConv();
        sprintf(buf, "%u", val0); // convert integer to string
        UART1_SendString("; IR:");
        UART1_SendString(buf);
        ADC_Deinit(); // Deinitialize ADC 
        LED_Blink_Twice();   
        tmr_wait_ms(TIMER1, 1000); // wait for 1 second before next sample
 
        //Scan Mode
        LED_Blink();
        ADC_Init_ScanMode(0x0820); // Initialize scan mode for AN5 and AN11
        uint16_t ir, bat;
        ADC_Start_ScanMode(&ir, &bat); // Start scan mode and get the conversion results
        char buf_ir[6], buf_bat[6];
        UART1_SendString("; IR:");
        sprintf(buf_ir, "%u", ir);// convert integer to string
        UART1_SendString(buf_ir);
        UART1_SendString("; BAT:");
        sprintf(buf_bat, "%u", bat);// convert integer to string
        UART1_SendString(buf_bat);
        LED_Blink_Twice();
        ADC_Deinit(); // Deinitialize ADC   
    }

    return 0;
}