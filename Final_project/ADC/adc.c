#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "uart.h"

void ADC_Init_MSamp_MConv(){
    AD1CON1bits.ADON = 0; // Disable ADC
    ANSELBbits.ANSB11 = 1; // Set RB11/AN11 as an analog
    TRISBbits.TRISB11 = 1; // Set RB11 as input 

    /* ADC setup for Manual Sampling and Manual Conversion */
    AD1CON1bits.ADON = 0; // Disable ADC 

    AD1CON3bits.ADCS = 8; // Tad configuration: ADC clock period, 8*Fcy, one every 8 instruction cycles
    
    AD1CON1bits.ASAM = 0; //manual sampling, sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0; //Start with sampling off
    AD1CON1bits.SSRC = 0; //manual conversion, conversion begins when SAMP bit is cleared AFTER sampling

    AD1CHS0bits.CH0SA = 11; //Select AN11 as input for CH0
    AD1CON2bits.CHPS = 0; //choose CH0

    AD1CON1bits.ADON = 1; // Enable ADC
}

void ADC_Init_MSamp_AConv(){
    AD1CON1bits.ADON = 0; // Disable ADC
    ANSELBbits.ANSB5 = 1; // Set RB5/AN5 as an analog input
    TRISBbits.TRISB5 = 1; // Set RB5 as input 

    /* ADC setup for Manual Sampling and Automatic Conversion */
    AD1CON1bits.ADON = 0; // Disable ADC 
    AD1CON3bits.ADCS = 8; // Tad configuration: ADC clock period, 8*Fcy, one every 8 instruction cycles
    AD1CON1bits.ASAM = 0; //manual sampling, sampling begins when SAMP bit is set
    AD1CON1bits.SSRC = 7; //automatic conversion, conversion begins immediately after sampling
    AD1CON3bits.SAMC = 16; // auto-sample time = 16 Tad
    AD1CHS0bits.CH0SA = 5; //Select AN5 as input for CH1
    AD1CON2bits.CHPS = 0; //choose CH0

    AD1CON1bits.ADON = 1; // Enable ADC
}

void ADC_Init_ScanMode(int pinToScan){ //for this assignment we will only scan AN5 and AN11, so 0x0820
    //here we both initialize the pins for the mikrobus and for the battery 
    AD1CON1bits.ADON = 0; // Disable ADC
    ANSELBbits.ANSB5 = 1; // Set RB5/AN5 as an analog input
    TRISBbits.TRISB5 = 1; // Set RB5 as input 
    ANSELBbits.ANSB11 = 1; // Set RB11/AN11 as an analog
    TRISBbits.TRISB11 = 1; // Set RA11 as input 
    
    AD1CON3bits.ADCS = 8; // Tad configuration: ADC clock period, 8*Fcy, one every 8 instruction cycles
    AD1CON3bits.SAMC = 16; // auto-sample time = 16 Tad
    AD1CON2bits.CSCNA = 1; // Enable scan mode
    AD1CON2bits.SMPI = 1; //interrupts after 2 conversions (since we are scanning 2 channels)
    AD1CON2bits.CHPS  = 0; // Scan CH0, scan mode uses only CH0
    AD1CSSL = pinToScan;   // bit 5 = AN5, bit 11 = AN11
    AD1CON1bits.SIMSAM = 0; // Sample channels sequentially, not simultaneously

    AD1CON1bits.ASAM = 1; //automatic sampling, sampling begins immediately after last conversion completes
    AD1CON1bits.SSRC = 7; //automatic conversion, conversion begins immediately after sampling

    AD1CON1bits.ADON = 0; // Keep ADC off until we are ready to sample and convert
}

uint16_t ADC_Start_MSamp_AConv(){
    AD1CON1bits.DONE   = 0; // Clear the DONE 
    AD1CON1bits.SAMP = 1; // Start sampling
    //here the sampling time is automatic   
    while(!AD1CON1bits.DONE); // Wait for conversion to complete
    
    AD1CON1bits.DONE   = 0; // Clear the DONE 

    return ADC1BUF0; // Return the conversion result
}

uint16_t ADC_Start_MSamp_MConv(){
// Sampling
    AD1CON1bits.DONE   = 0; // Clear the DONE 
    AD1CON1bits.SAMP = 1; // Start sampling
    if(tmr_wait_ms(TIMER1, 1) == 1){ // Sample for 1 ms
    AD1CON1bits.SAMP = 0; // stop sampling
    }
// Conversion
    while(!AD1CON1bits.DONE); // Wait for conversion to complete
    
    AD1CON1bits.DONE   = 0; // Clear the DONE 

    return ADC1BUF0; // Return the conversion result
}

void ADC_Start_ScanMode(uint16_t* ir, uint16_t* bat){
    AD1CON1bits.ADON = 1;
    IFS0bits.AD1IF = 0; // Clear ADC interrupt flag
    IEC0bits.AD1IE = 1; // Enable ADC interrupt    
    // Sampling and conversion are automatic in scan mode, so we just wait for the conversion to complete
    while(!IFS0bits.AD1IF); // Wait for all conversions to complete
    
    AD1CON1bits.DONE   = 0; // Clear the DONE 

    *ir = ADC1BUF0; // Store the conversion result of the first scanned channel (AN5)
    *bat = ADC1BUF1; // Store the conversion result of the second scanned channel (AN11)
}

void ADC_Deinit(){
    AD1CON1bits.ADON = 0; // Disable ADC
}