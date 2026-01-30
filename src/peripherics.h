#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

#define UP 1     // DSR|GPIO2[1]
#define LEFT 3   // GPIO0[3]
#define DOWN 7   // GIPO2[7]
#define RIGHT 8  // GIPO2[8]
#define SELECT 2 // GPIO0[2]

#define buttonS (1 << SELECT)
#define buttonU (1 << UP)
#define buttonD (1 << DOWN)
#define buttonL (1 << LEFT)
#define buttonR (1 << RIGHT)

enum {

	BS,
	BU,
	BD,
	BL,
	BR,
	NONE
};

void configureButtons();
unsigned char readButtons();
unsigned char readButtons2();
