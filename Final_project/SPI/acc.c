#include <p33EP512MU810.h>
#include <xc.h>
#include <math.h>
#include "acc.h"
#include "../TIMER/timer.h"
//constants and registers
#define ACC_REG_CHIPID 0x00
#define ACC_REG_ACCD_X_LSB 0x02
#define ACC_CHIPID_EXPECTED 0xFA //expected chip ID value
#define ACC_PMU_BW_REG 0x10
#define ACC_RESOLUTION 12
#define SPI_READ_BIT 0x80
#define BW_MIN 8
#define BW_MAX 15
#define RAD_TO_DEG (180.0f / 3.14159265f)

#define imu_select() (LATBbits.LATB3 = 0)
#define imu_deselect() (LATBbits.LATB3 = 1)

#define Mag_select() (LATDbits.LATD6 = 0)
#define Mag_deselect() (LATDbits.LATD6 = 1)

//SPI: MISO=RA1, MOSI=RF13, SCK=RF12, CS=RB3
//Config: 8-bit, mode 0, SCK = 72MHz / (16 * 4) = 1.125 MHz
void SPI_Init(void) {
    TRISAbits.TRISA1 = 1;
    TRISFbits.TRISF12 = 0;
    TRISFbits.TRISF13 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1;
    //Peripheral pin select: SDO1=RA1, SDI1=RF13, SCK1=RF12
    RPINR20bits.SDI1R = 0b0010001;
    RPOR12bits.RP109R = 0b000101;
    RPOR11bits.RP108R = 0b000110;
    //SPI master mode
    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.MODE16 = 0;
    SPI1CON1bits.PPRE = 1;
    SPI1CON1bits.SPRE = 4;
    SPI1STATbits.SPIEN = 1;
}
//transfer byte and return received byte (blocks until transfer complete)
unsigned char SPI_TransferByte(unsigned char tx_byte) {
    while (SPI1STATbits.SPITBF == 1);   //wait until transmit buffer is empty
    SPI1BUF = tx_byte;                  //write to transmit buffer
    while (SPI1STATbits.SPIRBF == 0);   //wait until receive buffer is full
    return (unsigned char)SPI1BUF;      //read received byte from the buffer
}

//Read chip ID register (0x00),we expect 0xFA
unsigned char ACC_ReadChipID(void) { 
    unsigned char chip_id;
    imu_select();
    SPI_TransferByte(SPI_READ_BIT | ACC_REG_CHIPID);
    chip_id = SPI_TransferByte(0x00);   //sending dummy value
    imu_deselect();
    return chip_id;
}
void Mag_WriteRegister(unsigned char reg, unsigned char value) {
    Mag_select();
    SPI_TransferByte(reg & 0x7F); 
    SPI_TransferByte(value);
    Mag_deselect();
}
void Mag_Init(void) {
    Mag_WriteRegister(0x4B, 0x01);
    tmr_wait_ms(TIMER1, 3);
    Mag_WriteRegister(0x4C, 0x00);
}
void Mag_ReadChipID(unsigned char *chip_id) { //only needed for reading the chip ID for testing
    Mag_select(); // change to the PORT connected to the chip select
    unsigned char read_addr = 0x40;
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = read_addr | 0x80; // setting the MSB to 1
    while (SPI1STATbits.SPIRBF == 0);
    unsigned char trash = SPI1BUF; // read to prevent buffer overrun
    while (SPI1STATbits.SPITBF == 1);
    SPI1BUF = 0x00; // clocking out zeros so that the other chip can send the value
    while (SPI1STATbits.SPIRBF == 0);
    *chip_id = SPI1BUF; // get the value from the register 0x32 (in decimal 50)
    Mag_deselect();
} 

static int mag_to_signed(unsigned char msb, unsigned char lsb) {
    int val = ((int)msb << 5) | (lsb >> 3);   //13-bit value
    if (val & (1 << 12)) { val -= (1 << 13); }
    return val;
}

void Mag_ReadAxes(int *x, int *y, int *z) {
    unsigned char x_lsb, x_msb, y_lsb, y_msb, z_lsb, z_msb;

    Mag_select();
    SPI_TransferByte(SPI_READ_BIT | 0x42);   
    x_lsb = SPI_TransferByte(0x00);
    x_msb = SPI_TransferByte(0x00);
    y_lsb = SPI_TransferByte(0x00);
    y_msb = SPI_TransferByte(0x00);
    z_lsb = SPI_TransferByte(0x00);
    z_msb = SPI_TransferByte(0x00);
    Mag_deselect();

    *x = mag_to_signed(x_msb, x_lsb);
    *y = mag_to_signed(y_msb, y_lsb);
    *z = mag_to_signed(z_msb, z_lsb);
}
void Mag_ComputeHeading(int *x, int *y, float *heading) {
    *heading = atan2f((float)(*y), (float)(*x)) * RAD_TO_DEG;
    if (*heading < 0) {
        *heading += 360.0f; // Normalize to [0, 360)
    }
}
//Setting bandwidth (8-15) to PMU_BW register
void ACC_SetBandwidth(unsigned char bw_value) {
    imu_select();
    SPI_TransferByte(ACC_PMU_BW_REG);
    SPI_TransferByte(bw_value);
    imu_deselect();
}
//documentation
static int acc_to_signed(unsigned char msb, unsigned char lsb){
    int val = (msb <<4 ) | (lsb >> 4);
    if (val & (1 << (ACC_RESOLUTION - 1))) {val -= (1 << ACC_RESOLUTION);}
    return val;
}
// Burst-read X/Y/Z axes (registers 0x02-0x07), assemble 12-bit signed values
void ACC_ReadAxes(int *x, int *y, int *z) {
    unsigned char x_lsb, x_msb, y_lsb, y_msb, z_lsb, z_msb;

    imu_select();
    //sending the starting register address with the read bit
    //all the other bytes sent are dummy bytes just to make the SPI clock run
    //and send us back the data bytes that we need to read
    SPI_TransferByte(SPI_READ_BIT | ACC_REG_ACCD_X_LSB);
    x_lsb = SPI_TransferByte(0x00);
    x_msb = SPI_TransferByte(0x00);
    y_lsb = SPI_TransferByte(0x00);
    y_msb = SPI_TransferByte(0x00);
    z_lsb = SPI_TransferByte(0x00);
    z_msb = SPI_TransferByte(0x00);

    imu_deselect();

    // Assemble 12-bit and sign-extend
    *x = acc_to_signed(x_msb, x_lsb);
    *y = acc_to_signed(y_msb, y_lsb);
    *z = acc_to_signed(z_msb, z_lsb);
    //using x,y,z pointers to return values
}
//computing roll and pitch
void ACC_ComputeAngles(int x, int y, int z,float *roll, float *pitch) {
    float fx = (float)x;
    float fy = (float)y;
    float fz = (float)z;

    *roll = (float)atan2f(fy, fz) * RAD_TO_DEG;
    *pitch = (float)atan2f(-fx, sqrtf(fy*fy + fz*fz)) * RAD_TO_DEG;
    //using pointers to return values
}