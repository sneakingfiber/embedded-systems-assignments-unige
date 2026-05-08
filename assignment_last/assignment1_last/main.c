/* =============================================================================
 * Assignment 1 — BMA280 Accelerometer + UART Control
 * Target: dsPIC33EP512MU810 @ 72 MHz
 *
 * Main loop: 100 Hz (TIMER1, 10 ms period)
 * Accel acquisition: 50 Hz (every 2nd iteration)
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

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */
#define MAIN_LOOP_HZ        100                          /* TIMER1 period: 10 ms            */
#define ACCEL_SAMPLE_HZ     50                           /* Acquire axes every 2 loop ticks */
#define ACCEL_SAMPLE_DIV    (MAIN_LOOP_HZ / ACCEL_SAMPLE_HZ)

#define ANGLE_OUTPUT_HZ     5                            /* Fixed $ANG transmit rate        */
#define ANGLE_OUTPUT_DIV    (MAIN_LOOP_HZ / ANGLE_OUTPUT_HZ)

#define RAD_TO_DEG          (180.0f / 3.14159265f)       /* Radian-to-degree conversion     */

#define BW_MIN              8                            /* Minimum valid bandwidth code    */
#define BW_MAX              15                           /* Maximum valid bandwidth code    */

#define ACC_REG_CHIPID      0x00                         /* BGW_CHIPID register address     */
#define ACC_REG_ACCD_X_LSB  0x02                         /* Burst-read start (X LSB)        */
#define ACC_CHIPID_EXPECTED 0xFA                         /* Expected chip ID per datasheet  */
#define ACC_PMU_BW_REG      0x10                         /* Bandwidth filter register       */
#define SPI_READ_BIT        0x80                         /* OR with address for SPI reads   */

#define RX_BUF_SIZE         16                           /* UART frame assembly buffer size */

/* ---------------------------------------------------------------------------
 * Global state
 * --------------------------------------------------------------------------- */
volatile int  g_uart_output_hz = 10;   /* UART transmit frequency (Hz)         */
static   int  g_hz_counter     = 0;    /* Tick counter for TX rate decimation  */

static int   g_accel_x       = 0;     /* Latest X-axis reading (12-bit signed) */
static int   g_accel_y       = 0;     /* Latest Y-axis reading (12-bit signed) */
static int   g_accel_z       = 0;     /* Latest Z-axis reading (12-bit signed) */
static int   g_accel_counter = 0;     /* Tick counter for 50 Hz decimation     */

static float g_roll_deg      = 0.0f;  /* Roll  angle in degrees                */
static float g_pitch_deg     = 0.0f;  /* Pitch angle in degrees                */
static int   g_angle_counter = 0;     /* Tick counter for 5 Hz angle output    */

static char  g_rx_buf[RX_BUF_SIZE];  /* UART incoming frame assembly buffer   */
static int   g_rx_idx        = 0;    /* Write index into g_rx_buf             */

static int   g_loop_ret      = 0;    /* Return value of tmr_wait_period       */

/* ---------------------------------------------------------------------------
 * Helper: returns non-zero if hz is one of the allowed $HZ values
 * --------------------------------------------------------------------------- */
static int is_valid_output_hz(int hz)
{
    return hz == 0 || hz == 1 || hz == 2 || hz == 5 || hz == 10;
}

/* ===========================================================================
 * SPI DRIVER
 *
 * Pin mapping:
 *   MISO : RA1  / RPI17  — accelerometer SDO
 *   MOSI : RF13 / RP109  — accelerometer SDI
 *   SCK  : RF12 / RP108  — clock
 *   CS   : RB3           — chip-select (active-low, manual GPIO)
 *
 * Configuration: 8-bit, SPI mode 0 (CKP=0, CKE=1)
 *   SCK = 72 MHz / (PPRE=16 x SPRE=4) = 1.125 MHz
 * =========================================================================== */

void SPI1_Init(void)
{
    /* Pin directions */
    TRISAbits.TRISA1  = 1;  /* RA1  = MISO (input)  */
    TRISFbits.TRISF12 = 0;  /* RF12 = SCK  (output) */
    TRISFbits.TRISF13 = 0;  /* RF13 = MOSI (output) */
    TRISBbits.TRISB3  = 0;  /* RB3  = CS   (output) */
    LATBbits.LATB3    = 1;  /* CS idle high          */

    /* Peripheral Pin Select (PPS) */
    RPINR20bits.SDI1R = 0b0010001;  /* RPI17 (RA1)  → SDI1 */
    RPOR12bits.RP109R = 0b000101;   /* RP109 (RF13) → SDO1 */
    RPOR11bits.RP108R = 0b000110;   /* RP108 (RF12) → SCK1 */

    /* SPI1 module configuration */
    SPI1CON1bits.MSTEN  = 1;  /* Master mode                       */
    SPI1CON1bits.CKP    = 0;  /* Clock idle low  (SPI mode 0)      */
    SPI1CON1bits.CKE    = 1;  /* Data valid on active-to-idle edge */
    SPI1CON1bits.MODE16 = 0;  /* 8-bit transfers                   */
    SPI1CON1bits.PPRE   = 1;  /* Primary  prescaler 16:1           */
    SPI1CON1bits.SPRE   = 4;  /* Secondary prescaler  4:1          */
    SPI1STATbits.SPIEN  = 1;  /* Enable SPI1                       */
}

/* Transmit one byte and return the simultaneously received byte */
static unsigned char SPI1_TransferByte(unsigned char tx_byte)
{
    while (SPI1STATbits.SPITBF == 1);  /* Wait until TX buffer is not full */
    SPI1BUF = tx_byte;
    while (SPI1STATbits.SPIRBF == 0);  /* Wait until RX buffer has data    */
    return (unsigned char)SPI1BUF;
}

/* ===========================================================================
 * BMA280 ACCELEROMETER DRIVER
 * =========================================================================== */

/* ---------------------------------------------------------------------------
 * ACC_ReadChipID
 * Reads register 0x00 (BGW_CHIPID). Returns 0xFA when the device is healthy.
 * --------------------------------------------------------------------------- */
unsigned char ACC_ReadChipID(void)
{
    unsigned char chip_id;

    LATBbits.LATB3 = 0;                               /* CS low               */
    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_CHIPID); /* Send read address    */
    chip_id = SPI1_TransferByte(0x00);                /* Clock in data byte   */
    LATBbits.LATB3 = 1;                               /* CS high              */

    return chip_id;  /* Expected: ACC_CHIPID_EXPECTED (0xFA = 250) */
}

/* ---------------------------------------------------------------------------
 * ACC_SetBandwidth
 * Writes bw_value to the PMU_BW register (0x10).
 * Valid range: BW_MIN (8) .. BW_MAX (15)
 * --------------------------------------------------------------------------- */
void ACC_SetBandwidth(unsigned char bw_value)
{
    unsigned char dummy;

    LATBbits.LATB3 = 0;                    /* CS low (write: address MSB = 0) */
    SPI1_TransferByte(ACC_PMU_BW_REG);     /* Register address                */
    dummy = SPI1_TransferByte(0x00);       /* Discard dummy                   */
    SPI1_TransferByte(bw_value);           /* Write bandwidth code             */
    dummy = SPI1_TransferByte(0x00);       /* Discard dummy                   */
    LATBbits.LATB3 = 1;                    /* CS high                         */
    (void)dummy;
}

/* ---------------------------------------------------------------------------
 * ACC_ReadAxes
 * Burst-reads registers 0x02–0x07 (X/Y/Z LSB + MSB) in one CS transaction.
 * Holding CS low for the whole burst locks the shadow register, ensuring that
 * each LSB and MSB pair belongs to the same conversion sample.
 *
 * Data format per axis (12-bit two's complement):
 *   raw = (MSB[7:0] << 4) | (LSB[7:4])
 * Sign-extended to 16-bit for XC16.
 * --------------------------------------------------------------------------- */
void ACC_ReadAxes(int *x, int *y, int *z)
{
    unsigned char x_lsb, x_msb, y_lsb, y_msb, z_lsb, z_msb;
    unsigned int  raw;

    LATBbits.LATB3 = 0; // CS low
    
    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_ACCD_X_LSB);  /* Send read address */
    // each subsequent SPI1_TransferByte(0x00) clocks out the next register in sequence
    x_lsb = SPI1_TransferByte(0x00);  /* 0x02  ACCD_X_LSB */
    x_msb = SPI1_TransferByte(0x00);  /* 0x03  ACCD_X_MSB */
    y_lsb = SPI1_TransferByte(0x00);  /* 0x04  ACCD_Y_LSB */
    y_msb = SPI1_TransferByte(0x00);  /* 0x05  ACCD_Y_MSB */
    z_lsb = SPI1_TransferByte(0x00);  /* 0x06  ACCD_Z_LSB */
    z_msb = SPI1_TransferByte(0x00);  /* 0x07  ACCD_Z_MSB */
    LATBbits.LATB3 = 1; // CS high

    /* Assemble 12-bit value and sign-extend to int (bit 11 set → negative) */
    raw = ((unsigned int)x_msb << 4) | (x_lsb >> 4);
    *x  = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;

    raw = ((unsigned int)y_msb << 4) | (y_lsb >> 4);
    *y  = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;

    raw = ((unsigned int)z_msb << 4) | (z_lsb >> 4);
    *z  = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;
}

/* ---------------------------------------------------------------------------
 * ACC_ComputeAngles
 *
 * Computes roll and pitch angles (degrees) from raw 12-bit accelerometer
 * readings using the standard atan2 equations:
 *
 *   roll  = atan2( y,  z )             — rotation around X-axis
 *   pitch = atan2(-x, sqrt(y² + z²) ) — rotation around Y-axis
 *
 * The scale factor cancels in the ratio, so raw counts are sufficient.
 * Output: roll ∈ [−180°, +180°],  pitch ∈ [−90°, +90°]
 * --------------------------------------------------------------------------- */
static void ACC_ComputeAngles(int raw_x, int raw_y, int raw_z,
                               float *p_roll, float *p_pitch)
{
    float fx = (float)raw_x;
    float fy = (float)raw_y;
    float fz = (float)raw_z;

    *p_roll  = atan2f(fy, fz)                     * RAD_TO_DEG;
    *p_pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz))  * RAD_TO_DEG;
}

/* ===========================================================================
 * ALGORITHM  (simulated 7 ms processing task)
 * =========================================================================== */
void algorithm(void)
{
    tmr_wait_ms(TIMER2, 7);
}

/* ===========================================================================
 * UART COMMAND PARSER
 *
 * Frame format:   $CC,VVV*
 *   [0]     '$'   — start delimiter
 *   [1..2]  CC    — 2-char command code ("BW" or "HZ")
 *   [3]     ','   — separator
 *   [4..]   VVV   — ASCII decimal value terminated by '*'
 *
 * Accepted commands:
 *   $BW,xx*  — set BMA280 bandwidth code (8..15)
 *   $HZ,yy*  — set ACC output rate       (0, 1, 2, 5, 10)
 *
 * Error response:
 *   $ERR,1*  — sent on malformed frame, unknown command, or out-of-range value
 * =========================================================================== */

/* ---------------------------------------------------------------------------
 * UART_ProcessFrame  (private)
 * Validates and dispatches a fully received frame stored in g_rx_buf.
 * --------------------------------------------------------------------------- */
static void UART_ProcessFrame(void)
{
    char *p_value_str;  /* Points to the value substring (after ',') */
    int   cmd_value;    /* Parsed numeric value                       */
    int   is_valid;     /* 1 = frame and value passed all checks      */
    int   char_idx;     /* Loop index for digit validation            */

    /* Step 1: Check minimum frame structure ($X,Y at positions 0 and 3) */
    if (g_rx_buf[0] != '$' || g_rx_buf[3] != ',') {
        return;  /* Silently discard — structure does not match protocol */
    }

    p_value_str = &g_rx_buf[4];  /* Value string begins immediately after ',' */
    is_valid    = 1;

    /* Step 2: Reject non-digit characters in the value field */
    for (char_idx = 0;
         p_value_str[char_idx] != '\0' && p_value_str[char_idx] != '*';
         char_idx++)
    {
        if (p_value_str[char_idx] < '0' || p_value_str[char_idx] > '9') {
            is_valid = 0;
            break;
        }
    }

    /* Step 3: Dispatch to the matching command handler */
    if (is_valid) {
        cmd_value = atoi(p_value_str);

        if (g_rx_buf[1] == 'B' && g_rx_buf[2] == 'W') {
            /* $BW,xx* — BMA280 bandwidth filter code (8..15) */
            if (cmd_value >= BW_MIN && cmd_value <= BW_MAX) {
                ACC_SetBandwidth((unsigned char)cmd_value);
            } else {
                is_valid = 0;
            }

        } else if (g_rx_buf[1] == 'H' && g_rx_buf[2] == 'Z') {
            /* $HZ,yy* — ACC data output rate (0, 1, 2, 5, 10 Hz) */
            if (is_valid_output_hz(cmd_value)) {
                g_uart_output_hz = cmd_value;
                g_hz_counter     = 0;  /* Prevent stale tick count */
            } else {
                is_valid = 0;
            }

        } else {
            is_valid = 0;  /* Unrecognised command code */
        }
    }

    /* Step 4: Report error if validation or dispatch failed */
    if (!is_valid) {
        UART1_SendString("$ERR,1*");
    }
}

/* ---------------------------------------------------------------------------
 * UART_ParseCommands  (public — called every main-loop iteration)
 *
 * Drains the circular RX buffer character-by-character and assembles frames
 * into g_rx_buf. On '*' reception the frame is passed to UART_ProcessFrame().
 *
 * Assembly rules:
 *   '$'  → reset buffer, start new frame
 *   '*'  → end of frame: null-terminate and process
 *   other chars → append while mid-frame and buffer has space
 * --------------------------------------------------------------------------- */
void UART_ParseCommands(void)
{
    char rx_char;

    /* RX overflow: ISR dropped a byte; discard partial frame and report */
    if (uart_rx_overflow) {
        uart_rx_overflow = 0;
        g_rx_idx = 0;
        UART1_SendString("$ERR,1*");
    }

    while (UART1_HasData()) {
        rx_char = UART1_ReceiveChar();

        if (rx_char == '$') {
            /* Start-of-frame: discard anything buffered previously */
            g_rx_idx = 0;
            g_rx_buf[g_rx_idx++] = rx_char;

        } else if (g_rx_idx > 0) {
            /* Mid-frame: accumulate if buffer space remains */
            if (g_rx_idx < (int)(sizeof(g_rx_buf) - 1)) {
                g_rx_buf[g_rx_idx++] = rx_char;
            }

            if (rx_char == '*') {
                /* End-of-frame: null-terminate, process, reset */
                g_rx_buf[g_rx_idx] = '\0';
                UART_ProcessFrame();
                g_rx_idx = 0;
            }
        }
        /* Characters before the first '$' are silently discarded */
    }
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

    /* Startup self-check: read and report BMA280 chip ID (expect 250 / 0xFA) */
    chip_id = ACC_ReadChipID();
    sprintf(tx_buf, "$CHIPID,%d*", chip_id);
    UART1_SendString(tx_buf);

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

        /* Step 3: Process any incoming UART commands */
        UART_ParseCommands();

        /* Step 4: Transmit $ACC,X,Y,Z* at the user-selected rate */
        if (g_uart_output_hz > 0) {
            g_hz_counter++;
            if (g_hz_counter >= (MAIN_LOOP_HZ / g_uart_output_hz)) {
                g_hz_counter = 0;
                sprintf(tx_buf, "$ACC,%d,%d,%d*",
                        g_accel_x, g_accel_y, g_accel_z);
                UART1_SendString(tx_buf);
            }
        }

        /* Step 5: Transmit $ANG,R,P* at fixed 5 Hz */
        g_angle_counter++;
        if (g_angle_counter >= ANGLE_OUTPUT_DIV) {
            g_angle_counter = 0;
            sprintf(tx_buf, "$ANG,%d,%d*",
                    (int)g_roll_deg, (int)g_pitch_deg);
            UART1_SendString(tx_buf);
        }

        /* Step 6: Block until end of 10 ms period */
        g_loop_ret = tmr_wait_period(TIMER1);
    }

    return 0;
}
