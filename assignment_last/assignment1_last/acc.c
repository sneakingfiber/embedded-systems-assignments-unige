#include <p33EP512MU810.h>
#include <xc.h>
#include <math.h>
#include "acc.h"

/* ---------------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------------- */
#define ACC_REG_CHIPID      0x00                         /* BGW_CHIPID register address     */
#define ACC_REG_ACCD_X_LSB  0x02                         /* Burst-read start (X LSB)        */
#define ACC_CHIPID_EXPECTED 0xFA                         /* Expected chip ID per datasheet  */
#define ACC_PMU_BW_REG      0x10                         /* Bandwidth filter register       */
#define SPI_READ_BIT        0x80                         /* OR with address for SPI reads   */
#define BW_MIN              8                            /* Minimum valid bandwidth code    */
#define BW_MAX              15                           /* Maximum valid bandwidth code    */

#define RAD_TO_DEG          (180.0f / 3.14159265f)       /* Radian-to-degree conversion     */

/* ---------------------------------------------------------------------------
 * Chip-select helpers (internal to accelerometer driver)
 * --------------------------------------------------------------------------- */
//This is just an helper function to make the code more readable 
static inline void acc_select(void)
{
    LATBbits.LATB3 = 0;  /* CS low  - activate   */
}

static inline void acc_deselect(void)
{
    LATBbits.LATB3 = 1;  /* CS high - deactivate */
}

/* ---------------------------------------------------------------------------
 * Global accelerometer data
 * --------------------------------------------------------------------------- */
int g_accel_x       = 0;     /* Latest X-axis reading (12-bit signed) */
int g_accel_y       = 0;     /* Latest Y-axis reading (12-bit signed) */
int g_accel_z       = 0;     /* Latest Z-axis reading (12-bit signed) */
int g_accel_counter = 0;     /* Tick counter for 50 Hz decimation     */

float g_roll_deg    = 0.0f;  /* Roll  angle in degrees                */
float g_pitch_deg   = 0.0f;  /* Pitch angle in degrees                */
int g_angle_counter = 0;     /* Tick counter for 5 Hz angle output    */

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

    /* Peripheral Pin Remapping */
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

    acc_select();
    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_CHIPID); /* Send read address    */
    chip_id = SPI1_TransferByte(0x00);                /* Clock in data byte   */
    acc_deselect();

    return chip_id;  /* Expected: ACC_CHIPID_EXPECTED (0xFA = 250) */
}

/* ---------------------------------------------------------------------------
 * ACC_SetBandwidth
 * Writes bw_value to the PMU_BW register (0x10).
 * Valid range: BW_MIN (8) .. BW_MAX (15)
 * --------------------------------------------------------------------------- */

 //DEBUGGING VERSION: reads back the register after writing and sends the value over UART
void ACC_SetBandwidth(unsigned char bw_value)
{
    unsigned char dummy;
    unsigned char read_back;
    char msg[24];

    /* --- Write bw_value to PMU_BW (0x10) --- */
    acc_select();
    dummy = SPI1_TransferByte(ACC_PMU_BW_REG);   /* address (MSB=0 → write) */
    dummy = SPI1_TransferByte(bw_value);         /* value to write          */
    acc_deselect();

    // THIS READING TAKES TOO MUCH TIME BECAUSE OF THE UART 
    //   FOR SURE DEADLINE MISSED IF DONE 
    // --- Debug: read PMU_BW back to verify the write took effect --- 
    acc_select();
    dummy     = SPI1_TransferByte(SPI_READ_BIT | ACC_PMU_BW_REG);
    read_back = SPI1_TransferByte(0x00);
    acc_deselect();

    /* Send the readback over UART so you can see it on the terminal. */
    sprintf(msg, "$BW_DBG,%u*", (unsigned)read_back);
    UART1_SendString(msg);
    //
    (void)dummy;
}

/* ---------------------------------------------------------------------------
 * ACC_ReadAxes
 * Burst-reads registers 0x02–0x07 (X/Y/Z LSB + MSB) in one CS transaction.
 * --------------------------------------------------------------------------- */
void ACC_ReadAxes(int *x, int *y, int *z)
{
    unsigned char x_lsb, x_msb, y_lsb, y_msb, z_lsb, z_msb;
    unsigned int  raw;

    acc_select();

    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_ACCD_X_LSB);  /* Send read address */
    // each subsequent SPI1_TransferByte(0x00) clocks out the next register in sequence
    x_lsb = SPI1_TransferByte(0x00);  /* 0x02  ACCD_X_LSB */
    x_msb = SPI1_TransferByte(0x00);  /* 0x03  ACCD_X_MSB */
    y_lsb = SPI1_TransferByte(0x00);  /* 0x04  ACCD_Y_LSB */
    y_msb = SPI1_TransferByte(0x00);  /* 0x05  ACCD_Y_MSB */
    z_lsb = SPI1_TransferByte(0x00);  /* 0x06  ACCD_Z_LSB */
    z_msb = SPI1_TransferByte(0x00);  /* 0x07  ACCD_Z_MSB */

    acc_deselect();

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
 * Output: roll = [−180°, +180°],  pitch = [−90°, +90°]
 * --------------------------------------------------------------------------- */
void ACC_ComputeAngles(int raw_x, int raw_y, int raw_z,
                       float *p_roll, float *p_pitch)
{
    float fx = (float)raw_x;
    float fy = (float)raw_y;
    float fz = (float)raw_z;

    *p_roll  = atan2f(fy, fz)                     * RAD_TO_DEG;
    *p_pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz))  * RAD_TO_DEG;
}
