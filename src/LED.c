// -----------------------------------------------------------------------
//  * File: LED.c
//      Library responsible for configuring and controlling the onboard LEDs.
//      This module provides simple routines to turn LEDs on, turn them off,
//      toggle their state and control them either by LED number or by GPIO/pin.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------

#include "LED.h"

void configureLED()
{
	// The application is expected to enable the required clocks beforehand.
	// Enable the clock for the GPIO module (UM10398, 3.5.14)
	// Enable the clock for the IOCON block (UM10398, 3.5.14)
	// (clocks.h)

	// Configure GPIO pins as outputs for the LEDs (UM10398, section 12.3.2)
	LPC_GPIO3->DIR |= (1 << PIN_LED1_4);
	LPC_GPIO1->DIR |= (1 << PIN_LED2);
	LPC_GPIO1->DIR |= (1 << PIN_LED3);
	LPC_GPIO1->DIR |= (1 << PIN_LED1_4);

	LPC_IOCON->R_PIO1_2 = 0x081;
	LPC_IOCON->R_PIO1_1 = 0x081;
	LPC_IOCON->R_PIO1_0 = 0x081;
}

// Turn OFF all LEDs
void turnOffLEDs()
{
	LPC_GPIO3->DATA &= (0 << PIN_LED1_4);
	LPC_GPIO1->DATA &= (0 << PIN_LED2);
	LPC_GPIO1->DATA &= (0 << PIN_LED3);
	LPC_GPIO1->DATA &= (0 << PIN_LED1_4);
}

// Turn ON all LEDs
void turnOnLEDs()
{
	LPC_GPIO3->DATA |= (1 << PIN_LED1_4);
	LPC_GPIO1->DATA |= (1 << PIN_LED2);
	LPC_GPIO1->DATA |= (1 << PIN_LED3);
	LPC_GPIO1->DATA |= (1 << PIN_LED1_4);
}

// Toggle all LEDs
void toggleLEDS()
{
	LPC_GPIO3->DATA ^= (1 << PIN_LED1_4);
	LPC_GPIO1->DATA ^= (1 << PIN_LED2);
	LPC_GPIO1->DATA ^= (1 << PIN_LED3);
	LPC_GPIO1->DATA ^= (1 << PIN_LED1_4);
}

// Uses the GPIO number and the LED pin
void turnOffLEDgpio(int gpio, int led)
{
	switch (gpio)
	{
	case 0:
		LPC_GPIO0->DATA &= (0 << led);
		break;
	case 1:
		LPC_GPIO1->DATA &= (0 << led);
		break;
	case 2:
		LPC_GPIO2->DATA &= (0 << led);
		break;
	case 3:
		LPC_GPIO3->DATA &= (0 << led);
		break;
	default:
		return;
	}
}

// Uses the LED number rather than the physical pin it is connected to
void turnOffLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA &= (0 << PIN_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA &= (0 << PIN_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA &= (0 << PIN_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA &= (0 << PIN_LED1_4);
		break;
	default:
		return;
	}
}

// Uses the LED number rather than the physical pin it is connected to
void turnOnLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA |= (1 << PIN_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA |= (1 << PIN_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA |= (1 << PIN_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA |= (1 << PIN_LED1_4);
		break;
	default:
		return;
	}
}

// Uses the LED number rather than the physical pin it is connected to
void toggleLED(int led)
{
	switch (led)
	{
	case 1:
		LPC_GPIO3->DATA ^= (1 << PIN_LED1_4);
		break;
	case 2:
		LPC_GPIO1->DATA ^= (1 << PIN_LED2);
		break;
	case 3:
		LPC_GPIO1->DATA ^= (1 << PIN_LED3);
		break;
	case 4:
		LPC_GPIO1->DATA ^= (1 << PIN_LED1_4);
		break;
	default:
		return;
	}
}
