// dsPIC33EP - BMA280 accelerometer over SPI, UART commands
// 100 Hz main loop, configurable UART output rate

#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"
#include "uart.h"
#include "acc.h"

#define MAIN_LOOP_HZ 100

// UART output control
volatile int g_uart_output_hz = 10;
volatile int g_hz_counter = 0;

static int loop_ret = 0;
static int deadline_misses = 0;



// ALGORITHM  (simulated 7 ms processing task)

void algorithm(void)
{
    tmr_wait_ms(TIMER2, 7);
}

int main(int argc, char **argv)
{
    char tx_buf[32];
    static char acc_buf[32] = "$ACC,";
    static char ang_buf[32] = "$ANG,";
    unsigned char chip_id;

    // disable analog on all ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // LED pins
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    LATAbits.LATA0 = 0;
    LATGbits.LATG9 = 0;

    UART1_Init();
    SPI1_Init();

    tmr_setup_period(TIMER1, 10);   // 100 Hz main loop
    tmr_setup_period(TIMER4, 500);  // LED blink

    __builtin_enable_interrupts();

    // DEBUG: chip ID read omitted (takes too much time via UART)
    // chip_id = ACC_ReadChipID();
    // sprintf(tx_buf, "$CHIPID,%d*", chip_id);
    // UART1_SendString(tx_buf);

    while (1) {
        // 7 ms processing task
        algorithm();

        // 50 Hz accelerometer acquisition (every 2nd iteration)
        g_accel_counter++;
        if (g_accel_counter >= ACCEL_SAMPLE_DIV) {
            g_accel_counter = 0;
            ACC_ReadAxes(&g_accel_x, &g_accel_y, &g_accel_z);
            ACC_ComputeAngles(g_accel_x, g_accel_y, g_accel_z,
                              &g_roll_deg, &g_pitch_deg);
        }

        // Check for incoming UART commands
        UART_ParseCommands();

        // Transmit $ACC,X,Y,Z* at configurable rate
        if (g_uart_output_hz > 0) {
            g_hz_counter++;
            if (g_hz_counter >= (MAIN_LOOP_HZ / g_uart_output_hz)) {
                g_hz_counter = 0;
                sprintf(acc_buf + 5, "%d,%d,%d*",
                        g_accel_x, g_accel_y, g_accel_z);
                UART1_SendString(acc_buf);
            }
        }

        // Transmit $ANG,roll,pitch* at fixed 5 Hz
        g_angle_counter++;
        if (g_angle_counter >= ANGLE_OUTPUT_DIV) {
            g_angle_counter = 0;
            sprintf(ang_buf + 5, "%d,%d*",
                    (int)g_roll_deg, (int)g_pitch_deg);
            UART1_SendString(ang_buf);
        }

        // Wait until end of 10 ms period
        g_loop_ret = tmr_wait_period(TIMER1);
        if (g_loop_ret == 1) {
            g_deadline_misses++;
            UART1_SendChar('M'); // deadline miss checker
        }
    }

    return 0;
}
