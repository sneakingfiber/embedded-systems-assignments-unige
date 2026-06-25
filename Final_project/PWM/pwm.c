#include "pwm.h"
#include <xc.h>
#include <p33EP512MU810.h>

void pwm_init(){
    //Setting up PWM pins as output
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    //PPS, tieing PWM output to pins
    RPOR0bits.RP65R = 16;  //OC1 -> RD1
    RPOR1bits.RP66R = 17;  //OC2 -> RD2
    RPOR1bits.RP67R = 18;  //OC3 -> RD3
    RPOR2bits.RP68R = 19;  //OC4 -> RD4
    //Configuring the 4 channels
    //OC1
    OC1CON1 = OC1CON2 = 0;//control register 1 
    OC1CON1bits.OCTSEL = 7; //internal clock 
    OC1CON2bits.SYNCSEL = 0x1F; //no sync source, coherent with the chosen clock source
    OC1RS = PWM_OC_PERIOD; //period register
    OC1R = 0; //initial duty cycle 0% (so it doesn't give pulses to the motors during conf)
    OC1CON1bits.OCM = 0b110; //edge aligned PWM

    //OC2
    OC2CON1 = OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 7; 
    OC2CON2bits.SYNCSEL = 0x1F; 
    OC2RS = PWM_OC_PERIOD;
    OC2R = 0; 
    OC2CON1bits.OCM = 0b110; 
    //OC3
    OC3CON1 = OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 7; 
    OC3CON2bits.SYNCSEL = 0x1F; 
    OC3RS = PWM_OC_PERIOD;
    OC3R = 0; 
    OC3CON1bits.OCM = 0b110; 
    //OC4
    OC4CON1 = OC4CON2 = 0;
    OC4CON1bits.OCTSEL = 7;
    OC4CON2bits.SYNCSEL = 0x1F; 
    OC4RS = PWM_OC_PERIOD;
    OC4R = 0; 
    OC4CON1bits.OCM = 0b110; 
} 


 void left_wheels_forward(uint8_t DutyCycle) {
    OC2R = ((uint32_t)DutyCycle * PWM_OC_PERIOD) / 100;  // RD2 = pwm
    OC1R = 0;              // RD1 = 0 
}
 void left_wheels_backward(uint8_t DutyCycle) {
    OC1R = ((uint32_t)DutyCycle * PWM_OC_PERIOD) / 100;  // RD1 = -pwm
    OC2R = 0;              // RD2 = 0
}
 void right_wheels_forward(uint8_t DutyCycle) {
    OC4R = ((uint32_t)DutyCycle * PWM_OC_PERIOD) / 100; // RD4 = pwm
    OC3R = 0;              // RD3 = 0
}
 void right_wheels_backward(uint8_t DutyCycle) {
    OC3R = ((uint32_t)DutyCycle * PWM_OC_PERIOD) / 100;  // RD3 = -pwm
    OC4R = 0;              // RD4 = 0
}
void motor_move(int leftDutyCycle, int rightDutyCycle) {
    //limiting the duty cycle so it doesn't go over 100% and change sign
    if (leftDutyCycle  >  100) leftDutyCycle  =  100;
    if (leftDutyCycle  < -100) leftDutyCycle  = -100;
    if (rightDutyCycle >  100) rightDutyCycle =  100;
    if (rightDutyCycle < -100) rightDutyCycle = -100;
    if (leftDutyCycle >= 0) {
        left_wheels_forward(leftDutyCycle);
       
    } else {
        left_wheels_backward(-leftDutyCycle);
    }
    if (rightDutyCycle >= 0) {
        right_wheels_forward(rightDutyCycle);
    } else {
        right_wheels_backward(-rightDutyCycle);
    }
}

void motor_stop(void) {
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;
}