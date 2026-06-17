// Accelerometer driver header
#ifndef ACC_H
#define ACC_H

void SPI1_Init(void);

unsigned char ACC_ReadChipID(void);
void ACC_SetBandwidth(unsigned char bw_value);
void ACC_ReadAxes(int *x, int *y, int *z);

void ACC_ComputeAngles(int raw_x, int raw_y, int raw_z,
                       float *p_roll, float *p_pitch);

extern int g_accel_x;
extern int g_accel_y;
extern int g_accel_z;
extern int g_accel_counter;

extern float g_roll_deg;
extern float g_pitch_deg;
extern int g_angle_counter;

#define ACCEL_SAMPLE_HZ 50
#define ACCEL_SAMPLE_DIV (100 / ACCEL_SAMPLE_HZ)
#define ANGLE_OUTPUT_HZ 5
#define ANGLE_OUTPUT_DIV (100 / ANGLE_OUTPUT_HZ)

#endif
