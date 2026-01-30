#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define TX  6 		// GPIO1_6
#define RX  7 		// GPIO1_7

void configureSerial();
char readSerial();
void writeSerial(char);
