// adc.h
#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <xc.h>
#include <p33EP512MU810.h>

#define ADC_VREF 3.3f
#define ADC_MAX_VALUE 1023.0f
#define BATTERY_DIVIDER_RATIO 3.0f // The battery voltage is divided by 3 before being read by the ADC

void ADC_Init_MSamp_MConv(void);
void ADC_Init_MSamp_AConv(void);
void ADC_Init_ScanMode(unsigned int pinToScan);
unsigned int ADC_Start_MSamp_MConv(void);
unsigned int ADC_Start_MSamp_AConv(void);
void ADC_Start_ScanMode(unsigned int* ir, unsigned int* bat);
void ADC_Deinit(void);
float adc_ir_to_cm(unsigned int adc_raw);
float adc_battery_voltage(unsigned int adc_raw);

#endif
