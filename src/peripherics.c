// -----------------------------------------------------------------------
//  * File: peripherics.c
//      Library for handling system peripherics. This module centralizes
//      low-level configuration and access to hardware resources such as
//      GPIOs, clocks and basic peripherals used across the system.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------



#include "peripherics.h" // LPC11xx Peripheral Registers

#define DEBOUNCECOUNT 3

unsigned char debounceCounter = 0;
int pressedButton, realPressedButton;
int previousButton = NONE;


/**
 * Configures all GPIO pins associated with the push buttons as inputs.
 *
 * - The direction bits are cleared so the pins operate in input mode.
 * - This allows the microcontroller to read the electrical level generated
 *   by each push button.
 *
 * This function should be called during the system initialization phase.
 */
void configureButtons()
{
    // Set button pins as input
    LPC_GPIO0->DIR &= ~buttonL;
    LPC_GPIO0->DIR &= ~buttonS;
    LPC_GPIO2->DIR &= ~buttonU;
    LPC_GPIO2->DIR &= ~buttonD;
    LPC_GPIO2->DIR &= ~buttonR;
}


/**
 * Reads the push buttons and returns the identifier of the detected button.
 *
 * - The function behaves like an edge-based detection. A button interaction
 *   is only reported when the button state transitions, instead of being
 *   reported continuously while it is held.
 * - In practice, the interaction is detected again only after the button is
 *   released and a new press occurs.
 *
 * The buttons are evaluated in a fixed priority order. The first valid
 * interaction found is returned. If no button is pressed, the function
 * returns NONE.
 */
unsigned char readButtons()
{
    int bS, bU, bD, bL, bR;

    if (!(LPC_GPIO0->DATA & buttonS)) bS = 0x01;
    if (!(LPC_GPIO0->DATA & buttonL)) bL = 0x01;
    if (!(LPC_GPIO2->DATA & buttonU)) bU = 0x01;
    if (!(LPC_GPIO2->DATA & buttonD)) bD = 0x01;
    if (!(LPC_GPIO2->DATA & buttonR)) bR = 0x01;


    if ((LPC_GPIO0->DATA & buttonS) &&  bS)
    {
    	bS = 0x00;
        return BS;
    }
    else if ((LPC_GPIO0->DATA & buttonL) && bL)
    {
        bL = 0x00;
        return BL;
    }
    else if ((LPC_GPIO2->DATA & buttonU) && bU)
    {
        bU = 0x00;
        return BU;
    }
    else if ((LPC_GPIO2->DATA & buttonD) && bD)
    {
        bD = 0x00;
        return BD;
    }
    else if ((LPC_GPIO2->DATA & buttonR) && bR)
    {
        bR = 0x00;
        return BR;
    }
    else
        return NONE;
}


/**
 * Reads the push buttons using a debounce mechanism based on state stability.
 *
 * - The current button state is compared with the previous one.
 * - A counter is incremented while both states remain equal.
 * - When the counter reaches the debounce threshold, the button press is
 *   considered valid.
 *
 * After the debounce threshold is reached, this function keeps reporting
 * the same button as long as it remains pressed. This results in repeated
 * actions when the user holds the button down, while still filtering out
 * mechanical bouncing effects.
 */
unsigned char readButtons2(){


    if      ((LPC_GPIO0->DATA & buttonS)) pressedButton =   BS;
    else if ((LPC_GPIO0->DATA & buttonL)) pressedButton =   BL;
    else if ((LPC_GPIO2->DATA & buttonU)) pressedButton =   BU;
    else if ((LPC_GPIO2->DATA & buttonD)) pressedButton =   BD;
    else if ((LPC_GPIO2->DATA & buttonR)) pressedButton =   BR;
    else 								  pressedButton = NONE;

    if(previousButton == pressedButton) debounceCounter++;
    else debounceCounter = 0;


    if (debounceCounter >= DEBOUNCECOUNT){
    	debounceCounter = 0;
    	realPressedButton = pressedButton;
    }
    else realPressedButton = NONE;


    previousButton = pressedButton;
    return realPressedButton;

}
