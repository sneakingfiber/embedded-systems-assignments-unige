#ifndef ADC_H
#define ADC_H
#include <stdint.h>

void ADC_Init(void);
void ADC_read(unsigned long  *ir_value, unsigned long  *vbat_value);
#endif // ADC_H