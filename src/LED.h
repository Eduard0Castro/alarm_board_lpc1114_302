#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define PIN_LED1_4 	    0 		// GPIO3[0]
#define PIN_LED3 		1	  	// GPIO1[1]
#define PIN_LED2 		2	  	// GPIO1[2]

void configureLED();
void turnOffLEDs();
void turnOnLEDs();
void toggleLEDS();
void turnOffLEDgpio(int, int);
void turnOffLED(int);
void turnOnLED(int);
void toggleLED(int);
