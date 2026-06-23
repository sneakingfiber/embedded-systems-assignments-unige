#include "pwm.h"
#define PWM_FREQ   10000UL
#define PWM_PERIOD ((FCY / PWM_FREQ) - 1)

void pwm_init(){
    //Setting up PWM pins as output
    TRISDbits.TRISD1 = TRISDbits.TRISD2 = TRISDbits.TRISD3 = TRISDbits.TRISD4 = 0;
    //PPS, tieing PWM output to pins
    RPOR0bits.RP65R = 16;  /// OC1 -> RD1
    RPOR1bits.RP66R = 17;  /// OC2 -> RD2
    RPOR1bits.RP67R = 18;  /// OC3 -> RD3
    RPOR2bits.RP68R = 19;  /// OC4 -> RD4
    //Configuring the 4 channels
    //OC1
    OC1CON1 = OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 7;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1RS = PWM_PERIOD;
    OC1R = 0;
    OC1CON1bits.OCM = 0b110;
    //OC2
    OC2CON1 = OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 7;
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2RS = PWM_PERIOD;
    OC2R = 0;
    OC2CON1bits.OCM = 0b110;
    //OC3
    OC3CON1 = OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 7;
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3RS = PWM_PERIOD;
    OC3R = 0;
    OC3CON1bits.OCM = 0b110;
    //OC4
    OC4CON1 = OC4CON2 = 0;
    OC4CON1bits.OCTSEL = 7;
    OC4CON2bits.SYNCSEL = 0x1F;
    OC4RS = PWM_PERIOD;
    OC4R = 0;
    OC4CON1bits.OCM = 0b110;
}

//function for setting the duty cycle of a specific channel (1-4) to a value between 0-100%
void set_duty(uint8_t channel, uint8_t value){
    uint16_t duty = (((uint32_t)value * PWM_PERIOD) / 100);
    switch(channel){
        case 1: OC1R = duty; break;
        case 2: OC2R = duty; break;
        case 3: OC3R = duty; break;
        case 4: OC4R = duty; break;
        default: break;
    }
}

void forward(uint8_t duty){
    set_duty(1, 0);
    set_duty(2, duty);
    set_duty(3, 0);
    set_duty(4, duty);
}
void backward(uint8_t duty){
    set_duty(1, duty);
    set_duty(2, 0);
    set_duty(3, duty);
    set_duty(4, 0);
}
void rotate_left(uint8_t duty){
    set_duty(1, duty);
    set_duty(2, 0);
    set_duty(3, 0);
    set_duty(4, duty);
}
void rotate_right(uint8_t duty){
    set_duty(1, 0);
    set_duty(2, duty);
    set_duty(3, duty);
    set_duty(4, 0);
}
void stop(void){
    set_duty(1, 0);
    set_duty(2, 0);
    set_duty(3, 0);
    set_duty(4, 0);
}
