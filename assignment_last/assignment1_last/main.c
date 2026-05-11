/* =============================================================================
 * Assignment 1 — BMA280 Accelerometer + UART Control
 * Target: dsPIC33EP512MU810 @ 72 MHz
 *
 * Main loop: 100 Hz (TIMER1, 10 ms period)
 * Accelerometer acquisition: 50 Hz (every 2 iteration)
 * UART output: configurable — 0 / 1 / 2 / 5 / 10 Hz
 *
 * UART commands received:
 *   $BW,xx*  — set accelerometer bandwidth (8..15)
 *   $HZ,yy*  — set output frequency (0, 1, 2, 5, 10)
 *
 * UART messages transmitted:
 *   $ACC,X,Y,Z*  — raw signed 12-bit accelerometer readings (at uart_freq Hz)
 *   $ANG,R,P*    — roll and pitch angles in degrees (fixed at 5 Hz)
 * ============================================================================= */

#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "uart.h"
#include "acc.h"

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */
#define MAIN_LOOP_HZ        100                          /* TIMER1 period: 10 ms            */

/* ---------------------------------------------------------------------------
 * Global state
 * --------------------------------------------------------------------------- */
volatile int  g_uart_output_hz = 10;   /* UART transmit frequency (Hz)         */
volatile int  g_hz_counter     = 0;    /* Tick counter for TX rate decimation  */

static int   g_loop_ret      = 0;    /* Return value of tmr_wait_period       */
static int g_deadline_misses = 0;   /* Debug counter for missed deadlines*/



// * ALGORITHM  (simulated 7 ms processing task)

void algorithm(void)
{
    tmr_wait_ms(TIMER2, 7);
}

/* ===========================================================================
 * MAIN
 * =========================================================================== */
int main(int argc, char **argv)
{
    char          tx_buf[32];
    unsigned char chip_id;

    /* Disable all analog inputs so digital I/O works on all ports */
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    /* LED output pins */
    TRISAbits.TRISA0 = 0;   /* RA0 = LD1 (output) */
    TRISGbits.TRISG9 = 0;   /* RG9 = LD2 (output) */
    LATAbits.LATA0   = 0;   /* LD1 off             */
    LATGbits.LATG9   = 0;   /* LD2 off             */

    /* Peripheral initialisation */
    UART1_Init();
    SPI1_Init();

    /* Timer setup */
    tmr_setup_period(TIMER1, 10);   /* 10 ms → 100 Hz main loop  */
    tmr_setup_period(TIMER4, 500);  /* 500 ms → spare / LD2 blink */

    __builtin_enable_interrupts();

    /* 
    DEBUG, commented because it takes too much time (UART)
    // Startup self-check: read and report BMA280 chip ID (expect 250 / 0xFA) 
    chip_id = ACC_ReadChipID(); //just for debugging 
    sprintf(tx_buf, "$CHIPID,%d*", chip_id);
    UART1_SendString(tx_buf); 
    */

    /* =========================================================================
     * MAIN CONTROL LOOP  (100 Hz, 10 ms period enforced by TIMER1)
     * ========================================================================= */
    while (1) {
        /* Step 1: Run simulated 7 ms processing task */
        algorithm();

        /* Step 2: Acquire accelerometer axes at 50 Hz (every 2nd iteration) */
        g_accel_counter++;
        if (g_accel_counter >= ACCEL_SAMPLE_DIV) {
            g_accel_counter = 0;
            ACC_ReadAxes(&g_accel_x, &g_accel_y, &g_accel_z);
            ACC_ComputeAngles(g_accel_x, g_accel_y, g_accel_z,
                              &g_roll_deg, &g_pitch_deg);
        }

        /* Process any incoming UART commands */
        UART_ParseCommands();

        /*  Transmit $ACC,X,Y,Z* at the selected rate */
        if (g_uart_output_hz > 0) {
            g_hz_counter++;
            if (g_hz_counter >= (MAIN_LOOP_HZ / g_uart_output_hz)) {
                g_hz_counter = 0;
                sprintf(tx_buf, "$ACC,%d,%d,%d*",
                        g_accel_x, g_accel_y, g_accel_z);
                UART1_SendString(tx_buf);
            }
        }

        /*  Transmit data through UART ($ANG,R,P*) at fixed 5 Hz */
        g_angle_counter++;
        if (g_angle_counter >= ANGLE_OUTPUT_DIV) {
            g_angle_counter = 0;
            sprintf(tx_buf, "$ANG,%d,%d*",
                    (int)g_roll_deg, (int)g_pitch_deg);
            UART1_SendString(tx_buf);
        }

        /*  Block until end of 10 ms period */
        g_loop_ret = tmr_wait_period(TIMER1);
        if (g_loop_ret == 1) {
            g_deadline_misses++;
            UART1_SendChar(g_deadline_misses % 10 + '0'); //debug: send the count of deadline misses over UART (mod 10 to keep it single-digit)
        }
        //debug: check if deadline misses are happening 
    }

    return 0;
}
