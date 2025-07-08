#ifndef ADC_H
#define	ADC_H

#include "LPC11xx.h"
#include "system_LPC11xx.h"

void ADC_Config();
void ADC_pin_config(unsigned char pin);
int ADC_read();

#endif