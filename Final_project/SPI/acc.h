// Accelerometer driver header
#ifndef ACC_H
#define ACC_H
#include <xc.h>
#include <p33EP512MU810.h>

void SPI_Init(void);
unsigned char SPI_TransferByte(unsigned char tx_byte);
unsigned char ACC_ReadChipID(void);
void ACC_SetBandwidth(unsigned char bw_value);
void ACC_ReadAxes(int *x, int *y, int *z);
void ACC_ComputeAngles(int raw_x, int raw_y, int raw_z, float *p_roll, float *p_pitch);

void Mag_Init(void);
void Mag_ReadAxes(int *x, int *y, int *z);
void Mag_ComputeHeading(int *x, int *y, float *heading);
void Mag_ReadChipID(unsigned char *chip_id);

#define ACCEL_SAMPLE_HZ 50
#define ACCEL_SAMPLE_DIV (100 / ACCEL_SAMPLE_HZ)
#define ANGLE_OUTPUT_HZ 5
#define ANGLE_OUTPUT_DIV (100 / ANGLE_OUTPUT_HZ)

#endif
