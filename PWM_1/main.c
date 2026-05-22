/**
 * @file    main.c
 * @brief   PWM motor control (forward & reverse) using Output Compare on PIC33EP512MU810.
 *
 * Button E8 cycles through motor states:
 *   Stopped -> Forward -> Reverse -> Stopped -> ...
 *
 * DRV8833 Table 2:
 *   Forward: xIN1 (RD2/RD4) = PWM,  xIN2 (RD1/RD3) = 0
 *   Reverse: xIN1 (RD2/RD4) = 0,    xIN2 (RD1/RD3) = PWM
 *
 * OC channel mapping:
 *   OC1 -> RD2 (RP66)  right motor xIN1
 *   OC2 -> RD4 (RP68)  left  motor xIN1
 *   OC3 -> RD1 (RP65)  right motor xIN2
 *   OC4 -> RD3 (RP67)  left  motor xIN2
 *
 * Clock:  FRC = 7.37 MHz, PLLFBD = 63 (M = 65), PLLPRE = 0 (N1 = 2),
 *         PLLPOST = 0 (N2 = 2) -> Fosc = 119.76 MHz -> Fcy = 59.88 MHz (~60 MIPS)
 */

#include <xc.h>
#include <p33EP512MU810.h>

/* --------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------- */

/** Instruction cycle frequency (Hz). Must match the PLL configuration. */
#define FCY             72000000UL

/**
 * OCxRS period value for 10 kHz PWM (internal OC clock, OCTSEL = 7).
 * Formula: OCxRS = Fcy / Fpwm - 1 = 59,880,000 / 10,000 - 1 = 5987
 * The OC counter resets when OCxTMR reaches this value.
 */
#define PWM_OC_PERIOD   5987U

/**
 * OCxR compare value for 50 % duty cycle.
 * Output is HIGH from OCxTMR = 0 to OCxR, then LOW to OCxRS.
 * 50 % is within the required 40-100 % range for motor motion.
 */
#define PWM_DUTY_50_PCT (PWM_OC_PERIOD / 2U)

/* Motor state values */
#define MOTOR_STOPPED   0U
#define MOTOR_FORWARD   1U
#define MOTOR_REVERSE   2U

/* --------------------------------------------------------------------------
 * Global state
 * -------------------------------------------------------------------------- */

/** Current motor state: MOTOR_STOPPED / MOTOR_FORWARD / MOTOR_REVERSE. */
volatile unsigned char g_motorState = MOTOR_STOPPED;

/* --------------------------------------------------------------------------
 * Function prototypes
 * -------------------------------------------------------------------------- */

void init_gpio(void);
void init_oc(void);
void motor_forward(void);
void motor_reverse(void);
void motor_stop(void);
void delay_ms(unsigned int ms);

/* --------------------------------------------------------------------------
 * main
 * -------------------------------------------------------------------------- */

int main(void) {

    init_gpio();
    init_oc();

    /* Pull-up on RE8: idle HIGH (1), pressed LOW (0). */
    unsigned char prevButtonState = 1;

    while (1) {
        unsigned char currButtonState = PORTEbits.RE8;

        /* Detect falling edge: button just pressed. */
        if (prevButtonState == 1 && currButtonState == 0) {

            delay_ms(50);   /* Debounce: wait 50 ms before confirming press. */

            if (PORTEbits.RE8 == 0) {

                /* Cycle: Stopped -> Forward -> Reverse -> Stopped. */
                if (g_motorState == MOTOR_STOPPED) {
                    motor_forward();
                    g_motorState = MOTOR_FORWARD;
                } else if (g_motorState == MOTOR_FORWARD) {
                    motor_reverse();
                    g_motorState = MOTOR_REVERSE;
                } else {
                    motor_stop();
                    g_motorState = MOTOR_STOPPED;
                }

                /* Wait for the button to be physically released. */
                while (PORTEbits.RE8 == 0) {
                    delay_ms(10);
                }
                delay_ms(50);   /* Debounce: settle time after release. */
            }
        }

        prevButtonState = currButtonState;
    }

    return 0;
}

/* --------------------------------------------------------------------------
 * Peripheral initialisation
 * -------------------------------------------------------------------------- */

/**
 * @brief Configure GPIO pins for button input and all four motor control pins.
 *
 * RE8      - digital input  (button E8, active-low with pull-up)
 * RD2/RD4  - digital output (xIN1 right/left -> OC1/OC2 via PPS)
 * RD1/RD3  - digital output (xIN2 right/left -> OC3/OC4 via PPS)
 */
void init_gpio(void) {
    /* RE8: digital input, disable analog. */
    TRISEbits.TRISE8 = 1;
    ANSELEbits.ANSE8 = 0;

    /* RD1-RD4: all outputs (PPS routes OC1-OC4 to these pins). */
    TRISDbits.TRISD1 = 0;   /* xIN2 right -> OC3 */
    TRISDbits.TRISD2 = 0;   /* xIN1 right -> OC1 */
    TRISDbits.TRISD3 = 0;   /* xIN2 left  -> OC4 */
    TRISDbits.TRISD4 = 0;   /* xIN1 left  -> OC2 */
}

/**
 * @brief Configure OC1-OC4 for edge-aligned PWM using the internal clock.
 *
 * OCTSEL = 7     -> internal Fcy peripheral clock (no external timer needed).
 * SYNCSEL = 0x1F -> no sync source; OCxTMR resets when equal to OCxRS.
 *
 * PPS mapping:
 *   _RP66R = 0x10  RD2/RP66 -> OC1  (xIN1 right)
 *   _RP68R = 0x11  RD4/RP68 -> OC2  (xIN1 left)
 *   _RP65R = 0x12  RD1/RP65 -> OC3  (xIN2 right)
 *   _RP67R = 0x13  RD3/RP67 -> OC4  (xIN2 left)
 *
 * Register roles:
 *   OCxRS = period compare  (OCxTMR resets here -> sets 10 kHz frequency)
 *   OCxR  = duty compare    (output goes LOW here -> sets duty cycle)
 *
 * All four channels start at 0 % duty (motors stopped).
 */
void init_oc(void) {
    // Configure PPS to map OC1-OC4 to the desired pins (RD1-RD4).
   
    RPOR1bits.RP66R = 0b010000; // Map OC1 to RP66 (RD2)
    RPOR0bits.RP65R = 0b010001; // Map OC2 to RP65 (RD1)
    RPOR2bits.RP68R = 0b010010; // Map OC3 to RP68 (RD4)
    RPOR1bits.RP67R = 0b010011; // Map OC4 to RP67 (RD3)
    
    

    

    /* --- OC1: xIN1 right (RD2) --- */
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL  = 7;      /* Internal Fcy clock. */
    OC1CON2bits.SYNCSEL = 0x1F;   /* No sync; OCxRS sets period. */
    OC1RS = PWM_OC_PERIOD;        /* Period: 10 kHz. */
    OC1R  = 0;                    /* Duty: 0 % (stopped). */
    OC1CON1bits.OCM = 0b110;      /* Edge-aligned PWM, no fault. */

    /* --- OC2: xIN1 left (RD4) --- */
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL  = 7;
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2RS = PWM_OC_PERIOD;
    OC2R  = 0;
    OC2CON1bits.OCM = 0b110;

    /* --- OC3: xIN2 right (RD1) --- */
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL  = 7;
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3RS = PWM_OC_PERIOD;
    OC3R  = 0;
    OC3CON1bits.OCM = 0b110;

    /* --- OC4: xIN2 left (RD3) --- */
    OC4CON1 = 0;
    OC4CON2 = 0;
    OC4CON1bits.OCTSEL  = 7;
    OC4CON2bits.SYNCSEL = 0x1F;
    OC4RS = PWM_OC_PERIOD;
    OC4R  = 0;
    OC4CON1bits.OCM = 0b110;
}

/* --------------------------------------------------------------------------
 * Motor control
 * -------------------------------------------------------------------------- */

/**
 * @brief Drive both motors forward.
 * DRV8833 Table 2: xIN1 = PWM, xIN2 = 0 -> Forward PWM, fast decay.
 */
void motor_forward(void) {
    OC1R = PWM_DUTY_50_PCT;   /* xIN1 right = PWM. */ //rd2
    OC2R = PWM_DUTY_50_PCT;   /* xIN1 left  = PWM. */ //rd4
    OC3R = 0;                 /* xIN2 right = 0.   */ //rd1
    OC4R = 0;                 /* xIN2 left  = 0.   */ //rd3
}

/**
 * @brief Drive both motors in reverse.
 * DRV8833 Table 2: xIN1 = 0, xIN2 = PWM -> Reverse PWM, fast decay.
 */
void motor_reverse(void) {
    OC1R = 0;                 /* xIN1 right = 0.   */
    OC2R = 0;                 /* xIN1 left  = 0.   */
    OC3R = PWM_DUTY_50_PCT;   /* xIN2 right = PWM. */
    OC4R = PWM_DUTY_50_PCT;   /* xIN2 left  = PWM. */
}

/**
 * @brief Stop both motors (all xIN = 0).
 */
void motor_stop(void) {
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;
}

/* --------------------------------------------------------------------------
 * Utility
 * -------------------------------------------------------------------------- */

/**
 * @brief Blocking software delay.
 * @param ms  Number of milliseconds to wait.
 *
 * Calibrated for Fcy ~60 MHz: 15 000 NOP iterations ~= 1 ms.
 */
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 15000U; j++) {
            __asm__ volatile ("nop");
        }
    }
}
