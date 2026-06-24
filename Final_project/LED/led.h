#ifndef LED_H
#define LED_H

#include <xc.h>
#include <stdint.h>
#include <p33EP512MU810.h>
// Function prototypes
void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blink(void);
void LED_Blink_Twice(void);
uint8_t Button_Read(void);
void Button_Wait_Release(void);

#endif // LED_H
