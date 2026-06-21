// adc.h
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC_Init_MSamp_MConv(void);
void ADC_Init_MSamp_AConv(void);
void ADC_Init_ScanMode(int pinToScan);
uint16_t ADC_Start_MSamp_MConv(void);
uint16_t ADC_Start_MSamp_AConv(void);
void ADC_Start_ScanMode(uint16_t* ir, uint16_t* bat);
void ADC_Deinit(void);
float adc_ir_to_cm(uint16_t adc_raw);
float adc_battery_voltage(uint16_t adc_raw);

#endif
