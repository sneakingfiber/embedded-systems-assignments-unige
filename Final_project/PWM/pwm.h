
#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <p33EP512MU810.h>


#define FCY             72000000UL

#define PWM_OC_PERIOD   7199U


void pwm_init(void);


void move_forward(uint8_t DutyCycle);


void move_backward(uint8_t DutyCycle);

void turn_right(uint8_t DutyCycle);

void turn_left(uint8_t DutyCycle);

void left_wheels_forward(uint8_t DutyCycle);
void right_wheels_forward(uint8_t DutyCycle);
void left_wheels_backward(uint8_t DutyCycle);
void right_wheels_backward(uint8_t DutyCycle);

void motor_move(int8_t leftDutyCycle, int8_t rightDutyCycle);
void motor_stop(void);

#endif /* PWM_H */