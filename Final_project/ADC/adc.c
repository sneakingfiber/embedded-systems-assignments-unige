#include <xc.h>
#include <p33EP512MU810.h>
#include "adc.h"
#include <stdint.h>
void ADC_Init(void)
{
    TRISBbits.TRISB5  = 1;  ANSELBbits.ANSB5  = 1;   /* AN5  – IR  */
    TRISBbits.TRISB11 = 1;  ANSELBbits.ANSB11 = 1;   /* AN11 – BAT */

    AD1CON1bits.ADON  = 0;
    AD1CON1bits.AD12B = 1;   /* 12-bit                          */
    AD1CON1bits.FORM  = 0;   /* unsigned integer                */
    AD1CON1bits.SSRC  = 7;   /* auto-convert after SAMC TAD    */
    AD1CON1bits.ASAM  = 1;   /* auto-sample (continuous scan)  */
    AD1CON2bits.VCFG  = 0;   /* Vref = AVDD/AVSS (3.3 V)       */
    AD1CON2bits.CSCNA = 1;   /* channel scan                   */
    AD1CON2bits.SMPI  = 1;   /* flag after every 2-ch scan     */
    AD1CON3bits.ADCS  = 63;  /* TAD ≈ 1.78 µs                  */
    AD1CON3bits.SAMC  = 31;  /* 31 TAD sample time             */
    AD1CSSL = (1 << 5) | (1 << 11);   /* AN5 then AN11          */
   
    // clear ADC interrupt flag and enable ADC
    IFS0bits.AD1IF = 0;
    
    AD1CON1bits.ADON = 1;// enable ADC
}

void ADC_read(unsigned long  *ir_value, unsigned long  *vbat_value)
{
    unsigned long long sum_ir = 0, sum_vbat = 0;
    while (!IFS0bits.AD1IF);   /* wait for 2-channel scan */
    IFS0bits.AD1IF = 0;
    //take yhe average of the two channels and store them in the variables
    for (int i = 0; i <100 ; i++) {
        sum_ir += (ADC1BUF0 & 0x0FFF);  /* 12-bit result in lower 12 bits */
        sum_vbat += (ADC1BUF1 & 0x0FFF);
       
    }

    *ir_value  = (sum_ir / 100);  /* 12-bit result in lower 12 bits */
    *vbat_value = (sum_vbat / 100);
}
