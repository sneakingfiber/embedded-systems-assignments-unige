
#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "uart.h"

void ACC_Init_MSamp_MConv(){
    AD1CON1bits.ADON = 0; // Disable ADC
    ANSELBbits.ANSB11 = 1; // Set RB11/AN11 as an analog input

    /* ADC setup for Manual Sampling and Manual Conversion */
    AD1CONbits.ADON = 0; // Disable ADC 

    AD1CON3bits.ADCS = 8; // Tad configuration: ADC clock period, 8*Fcy, one every 8 instruction cycles
    
    AD1CON1bits.ASAM = 0; //manual sampling, sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0; //Start with sampling off
    AD1CON1bits.SSRC = 0; //manual conversion, conversion begins when SAMP bit is cleared AFTER sampling

    AD1CHS0bits.CH0SA = 11; //Select AN11 as input for CH0
    AD1CON2bits.CHPS = 0; //choose CH0

    AD1CON1bits.ADON = 1; // Enable ADC
}

void ACC_Init_MSamp_AConv(){
    AD1CON1bits.ADON = 0; // Disable ADC
    ANSELBbits.ANSB5 = 1; // Set RB5/AN5 as an analog input

    /* ADC setup for Manual Sampling and Automatic Conversion */
    AD1CONbits.ADON = 0; // Disable ADC 
    AD1CON3bits.ADCS = 8; // Tad configuration: ADC clock period, 8*Fcy, one every 8 instruction cycles
    AD1CON1bits.ASAM = 0; //manual sampling, sampling begins when SAMP bit is set
    AD1CON1bits.SSRC = 7; //automatic conversion, conversion begins immediately after sampling
    AD1CON3bits.SAMC = 16;
    AD1CHS0bits.CH0SA = 5; //Select AN5 as input for CH1
    AD1CON2bits.CHPS = 0; //choose CH0

    AD1CON1bits.ADON = 1; // Enable ADC
}

void ACC_Start_MSamp_AConv(){
    AD1CON1bits.DONE   = 0; // Clear the DONE 
    AD1CON1bits.SAMP = 1; // Start sampling
    if(timer_wait_ms(TIMER1, 1)){ // Sample for 1 ms
    AD1CON1bits.SAMP = 0; // stop sampling, this will also trigger conversion due to SSRC=7
    }
    
    while(!AD1CON1bits.DONE); // Wait for conversion to complete
    
    AD1CON1bits.DONE   = 0; // Clear the DONE 

    return ADC1BUF0; // Return the conversion result
}

void ACC_Start_MSamp_MConv(){
// Sampling
    AD1CON1bits.DONE   = 0; // Clear the DONE 
    AD1CON1bits.SAMP = 1; // Start sampling
    if(timer_wait_ms(TIMER1, 1)){ // Sample for 1 ms
    AD1CON1bits.SAMP = 0; // stop sampling
    }
// Conversion
    AD1CON1bits.SSRC = 1; // Start conversion
    while(!AD1CON1bits.DONE); // Wait for conversion to complete
    
    AD1CON1bits.DONE   = 0; // Clear the DONE 

    return ADC1BUF0; // Return the conversion result

}