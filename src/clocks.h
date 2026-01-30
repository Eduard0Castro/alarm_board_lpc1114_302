// -----------------------------------------------------------------------
//  * File: clock.h
//      Library responsible for configuring and enabling the system clocks.
//      This module centralizes the activation of peripheral clocks required
//      for GPIO, IOCON, I2C, UART and ADC operation.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------


#include "LPC11xx.h"
#include "system_LPC11xx.h"

/**
 * This function enables the clock for the GPIO module by setting the
 * corresponding bit in the SYSAHBCLKCTRL register. Enabling this clock
 * is mandatory before any GPIO pin can be configured or accessed.
 */
void clockGpio(){

    // Enable the clock for the GPIO module according to the LPC reference manual
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

}


/**
 * This function enables the clock for the IOCON block. The IOCON clock
 * must be enabled before configuring pin functions, pull-ups, pull-downs
 * or selecting alternate peripheral functions for the pins.
 */
void clockIocon(){

    // Enable the clock for the IOCON block according to the LPC reference manual
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);

}


/**
 * This function enables the clock for the I2C peripheral. Once the clock
 * is enabled, the I2C module can be configured and used for serial
 * communication with external devices.
 */
void clockI2c(){

	// Enable the clock for the I2C block according to the LPC reference manual
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);

}


/**
 * This function enables the clock for the UART peripheral and configures
 * the UART clock divider. The divider value is selected so that, based on
 * the system clock, a baud rate of approximately 19.2 kbps can be generated.
 */
void clockUart(){

	// Enable the clock for the UART block according to the LPC reference manual
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);
	// Set the UART module clock divider to generate a baud rate of 19.2 kbps
	LPC_SYSCON->UARTCLKDIV |= 0x9B;

}


/**
 * This function enables the clock for the ADC peripheral. The ADC clock
 * must be enabled before powering up the ADC and performing any
 * analog-to-digital conversions.
 */
void clockAdc(){

	// Enable the clock for the ADC block according to the LPC reference manual
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 13);

}


/**
 * This function performs the global clock configuration for the system.
 * It sequentially enables the clocks for GPIO, IOCON, I2C, UART and ADC,
 * ensuring that all required peripherals are powered and ready to be
 * configured and used by the application.
 */
void clocksConfig(){

	clockGpio();
	clockIocon();
	clockI2c();
	clockUart();
	clockAdc();
}
