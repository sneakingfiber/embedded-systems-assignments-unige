
#include "pwm.h"


void pwm_init(void) {

    RPOR1bits.RP66R = 0b010000;  /* OC1 -> RP66 (RD2)  xIN1 right motor */
    RPOR2bits.RP68R = 0b010001;  /* OC2 -> RP68 (RD4)  xIN1 left  motor */
    RPOR0bits.RP65R = 0b010010;  /* OC3 -> RP65 (RD1)  xIN2 right motor */
    RPOR1bits.RP67R = 0b010011;  /* OC4 -> RP67 (RD3)  xIN2 left  motor */

    /* ------------------------------------------------------------------
     * OC1 : xIN1 right motor  ->  RD2 (RP66)
     * ------------------------------------------------------------------ */
    OC1CON1 = 0;                      /* Clear all control bits.            */
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL  = 7;          /* Clock: internal Fcy (72 MHz).      */
    OC1CON2bits.SYNCSEL = 0x1F;       /* No sync; OCxRS sets period.        */
    OC1RS = PWM_OC_PERIOD;            /* Period = 7199 -> 10 kHz.           */
    OC1R  = 0;                        /* Duty   = 0 %  -> motor stopped.    */
    OC1CON1bits.OCM = 0b110;          /* Edge-aligned PWM, no fault pin.    */

    /* ------------------------------------------------------------------
     * OC2 : xIN1 left  motor  ->  RD4 (RP68)
     * ------------------------------------------------------------------ */
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL  = 7;
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2RS = PWM_OC_PERIOD;
    OC2R  = 0;
    OC2CON1bits.OCM = 0b110;

    /* ------------------------------------------------------------------
     * OC3 : xIN2 right motor  ->  RD1 (RP65)
     * ------------------------------------------------------------------ */
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL  = 7;
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3RS = PWM_OC_PERIOD;
    OC3R  = 0;
    OC3CON1bits.OCM = 0b110;

    /* ------------------------------------------------------------------
     * OC4 : xIN2 left  motor  ->  RD3 (RP67)
     * ------------------------------------------------------------------ */
    OC4CON1 = 0;
    OC4CON2 = 0;
    OC4CON1bits.OCTSEL  = 7;
    OC4CON2bits.SYNCSEL = 0x1F;
    OC4RS = PWM_OC_PERIOD;
    OC4R  = 0;
    OC4CON1bits.OCM = 0b110;
}


void move_forward(uint8_t DutyCycle) {
    OC1R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 right = 100 % PWM  (RD2). */
    OC2R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 left  = 100 % PWM  (RD4). */
    OC3R = 0;              /* xIN2 right = 0           (RD1). */
    OC4R = 0;              /* xIN2 left  = 0           (RD3). */
}

/**
 * @brief  Reverse – both motors full speed.
 *         OC1/OC2 (xIN1) = 0 %,  OC3/OC4 (xIN2) = 100 %.
 */
void move_backward(uint8_t DutyCycle) {
    OC1R = 0;              /* xIN1 right = 0           (RD2). */
    OC2R = 0;              /* xIN1 left  = 0           (RD4). */
    OC3R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN2 right = 100 % PWM  (RD1). */
    OC4R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN2 left  = 100 % PWM  (RD3). */
}
void turn_right(uint8_t DutyCycle) {
    OC1R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 right = 100 % PWM  (RD2). */
    OC2R = PWM_OC_PERIOD * ((float)DutyCycle / 50); //         (RD4).
    OC3R = 0;              /* xIN2 right = 0           (RD1). */
    OC4R = 0;              /* xIN2 left  = 0           (RD3). */
}

void turn_left(uint8_t DutyCycle) {
    OC1R = PWM_OC_PERIOD * ((float)DutyCycle / 50);  /* xIN1 right = 0           (RD2). */
    OC2R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 left  = 100 % PWM  (RD4). */
    OC3R = 0;              /* xIN2 right = 0           (RD1). */
    OC4R = 0;              /* xIN2 left  = 0           (RD3). */
}
 void left_wheels_forward(uint8_t DutyCycle) {
    OC1R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 right = 100 % PWM  (RD2). */
    OC3R = 0;              /* xIN2 right = 0           (RD1). */
}
void left_wheels_backward(uint8_t DutyCycle) {
    OC1R = 0;              /* xIN1 right = 0           (RD2). */
    OC3R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN2 right = 100 % PWM  (RD1). */
}
void right_wheels_forward(uint8_t DutyCycle) {
    OC2R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN1 left  = 100 % PWM  (RD4). */
    OC4R = 0;              /* xIN2 left  = 0           (RD3). */
}
void right_wheels_backward(uint8_t DutyCycle) {
    OC2R = 0;              /* xIN1 left  = 0           (RD4). */
    OC4R = PWM_OC_PERIOD * ((float)DutyCycle / 100);  /* xIN2 left  = 100 % PWM  (RD3). */
}
void motor_move(int8_t leftDutyCycle, int8_t rightDutyCycle) {
    if (leftDutyCycle >= 0) {
        left_wheels_forward(leftDutyCycle);
       
    } else {
        left_wheels_backward(-1 * leftDutyCycle);
    }
    if (rightDutyCycle >= 0) {
        right_wheels_forward(rightDutyCycle);
    } else {
        right_wheels_backward(-1 * rightDutyCycle);
    }
}

void motor_stop(void) {
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;
}
