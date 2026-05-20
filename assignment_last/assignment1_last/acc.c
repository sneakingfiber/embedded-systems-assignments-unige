// BMA280 accelerometer driver over SPI
#include <p33EP512MU810.h>
#include <xc.h>
#include <math.h>
#include "acc.h"

#define ACC_REG_CHIPID 0x00
#define ACC_REG_ACCD_X_LSB 0x02
#define ACC_CHIPID_EXPECTED 0xFA
#define ACC_PMU_BW_REG 0x10
#define SPI_READ_BIT 0x80
#define BW_MIN 8
#define BW_MAX 15
#define RAD_TO_DEG (180.0f / 3.14159265f)

#define acc_select() (LATBbits.LATB3 = 0)
#define acc_deselect() (LATBbits.LATB3 = 1)

int g_accel_x = 0;
int g_accel_y = 0;
int g_accel_z = 0;
int g_accel_counter = 0;

float g_roll_deg = 0.0f;
float g_pitch_deg = 0.0f;
int g_angle_counter = 0;

// SPI1: MISO=RA1, MOSI=RF13, SCK=RF12, CS=RB3
// Config: 8-bit, mode 0, SCK = 72MHz / (16 * 4) = 1.125 MHz

void SPI1_Init(void) {
    TRISAbits.TRISA1 = 1;
    TRISFbits.TRISF12 = 0;
    TRISFbits.TRISF13 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1;

    RPINR20bits.SDI1R = 0b0010001;
    RPOR12bits.RP109R = 0b000101;
    RPOR11bits.RP108R = 0b000110;

    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.MODE16 = 0;
    SPI1CON1bits.PPRE = 1;
    SPI1CON1bits.SPRE = 4;
    SPI1STATbits.SPIEN = 1;
}

static unsigned char SPI1_TransferByte(unsigned char tx_byte) {
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = tx_byte;
    while (SPI1STATbits.SPIRBF == 0);
    return (unsigned char)SPI1BUF;
}

// Read chip ID register (0x00), expect 0xFA
unsigned char ACC_ReadChipID(void) {
    unsigned char chip_id;

    acc_select();
    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_CHIPID);
    chip_id = SPI1_TransferByte(0x00);
    acc_deselect();

    return chip_id;
}

// Write bandwidth code (8-15) to PMU_BW register
void ACC_SetBandwidth(unsigned char bw_value) {
    unsigned char dummy;

    acc_select();
    dummy = SPI1_TransferByte(ACC_PMU_BW_REG);
    dummy = SPI1_TransferByte(bw_value);
    acc_deselect();
}

// Burst-read X/Y/Z axes (registers 0x02-0x07), assemble 12-bit signed values
void ACC_ReadAxes(int *x, int *y, int *z) {
    unsigned char x_lsb, x_msb, y_lsb, y_msb, z_lsb, z_msb;
    unsigned int raw;

    acc_select();

    SPI1_TransferByte(SPI_READ_BIT | ACC_REG_ACCD_X_LSB);
    x_lsb = SPI1_TransferByte(0x00);
    x_msb = SPI1_TransferByte(0x00);
    y_lsb = SPI1_TransferByte(0x00);
    y_msb = SPI1_TransferByte(0x00);
    z_lsb = SPI1_TransferByte(0x00);
    z_msb = SPI1_TransferByte(0x00);

    acc_deselect();

    // Assemble 12-bit and sign-extend
    raw = ((unsigned int)x_msb << 4) | (x_lsb >> 4);
    *x = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;

    raw = ((unsigned int)y_msb << 4) | (y_lsb >> 4);
    *y = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;

    raw = ((unsigned int)z_msb << 4) | (z_lsb >> 4);
    *z = (raw & 0x0800) ? (int)(raw | 0xF000) : (int)raw;
}

// Compute roll/pitch angles (degrees) from accelerometer readings
// roll = atan2(y, z), pitch = atan2(-x, sqrt(y^2 + z^2))
void ACC_ComputeAngles(int raw_x, int raw_y, int raw_z,
                       float *p_roll, float *p_pitch) {
    float fx = (float)raw_x;
    float fy = (float)raw_y;
    float fz = (float)raw_z;

    *p_roll = atan2f(fy, fz) * RAD_TO_DEG;
    *p_pitch = atan2f(-fx, sqrtf(fy*fy + fz*fz)) * RAD_TO_DEG;
}
