#include "led.h"
#include "../TIMER/timer.h"
#include<xc.h>
#include <p33EP512MU810.h>
void LED_Init(void)
{
    // Disable analog ports 
   ANSELA = 0; // Disable analog input on RA0
   
    //Set LED pins as outputs (RA0 and RG9)
    TRISAbits.TRISA0 = 0;
}

void LED_On(void)
{
    LATAbits.LATA0 = 1;
}

void LED_Off(void)
{
    LATAbits.LATA0 = 0;
}

void LED_Toggle(void)
{
    LATAbits.LATA0 = !LATAbits.LATA0;
}

void LED_Blink(void)
{
    LED_On();
    tmr_wait_ms(TIMER1, 500);
    LED_Off();
    tmr_wait_ms(TIMER1, 500);
}

void LED_Blink_Twice(void)
{
    LED_On();
    tmr_wait_ms(TIMER1, 250);
    LED_Off();
    tmr_wait_ms(TIMER1, 250);
    LED_On();
    tmr_wait_ms(TIMER1, 250);
    LED_Off();
    tmr_wait_ms(TIMER1, 250);
}

