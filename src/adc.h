#ifndef ADC_H
#define	ADC_H

#include "LPC11xx.h"
#include "system_LPC11xx.h"

void ADCconfig();
void ADCpinConfig(unsigned char pin);
int ADCread();

#endif