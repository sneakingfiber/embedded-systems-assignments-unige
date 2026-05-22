#include "led.h"

void LED_Init(void)
{
    // Disable analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Set LED pins as outputs (RA0 and RG9)
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;

    // Set button pin as input (RE8)
    TRISEbits.TRISE8 = 1;
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
    timer_wait_ms(500);
    LED_Off();
    timer_wait_ms(500);
}

void LED_Blink_Twice(void)
{
    LED_On();
    timer_wait_ms(250);
    LED_Off();
    timer_wait_ms(250);
    LED_On();
    timer_wait_ms(250);
    LED_Off();
    timer_wait_ms(250);
}

uint8_t Button_Read(void)
{
    return PORTEbits.RE8;
}

void Button_Wait_Release(void)
{
    while (PORTEbits.RE8 == 0)
    {
        // Wait for button to be released
    }
}
