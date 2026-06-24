#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <p33EP512MU810.h>

#define FCY 72000000UL

void pwm_init(void);
void set_duty(uint8_t channel, uint8_t value);
void forward(uint8_t duty);
void backward(uint8_t duty);
void rotate_left(uint8_t duty);
void rotate_right(uint8_t duty);
void stop(void);
void motor_move(uint8_t left, uint8_t right);

#endif /* PWM_H */
