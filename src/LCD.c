// -----------------------------------------------------------------------
//  * File: LCD.c
//      Library responsible for driving a character LCD using 4-bit mode.
//      This module handles pin configuration, command and data transmission,
//      cursor control and basic data formatting for display output.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------


#include "LCD.h"

/**
 * Configures all GPIO pins used by the LCD.
 * The pins are set as outputs and mapped to
 * standard GPIO functionality with no alternate functions.
 */
void LCDpinConfig(){

    // PIN DIRECTION CONFIGURATION
    LCD_RS_PORT->DIR |= (1 << LCD_RS_PIN);
    LCD_E_PORT->DIR  |= (1 << LCD_E_PIN);
    LCD_D4_PORT->DIR |= (1 << LCD_D4_PIN);
    LCD_D5_PORT->DIR |= (1 << LCD_D5_PIN);
    LCD_D6_PORT->DIR |= (1 << LCD_D6_PIN);
    LCD_D7_PORT->DIR |= (1 << LCD_D7_PIN);

    // IOCON CONFIGURATION (no alternate function)
    LPC_IOCON->PIO3_1  = 0x00; // RS
    LPC_IOCON->PIO1_4  = 0x80; // E
    LPC_IOCON->PIO1_11 = 0x80; // D4
    LPC_IOCON->PIO3_2  = 0x00; // D5
    LPC_IOCON->PIO1_5  = 0x00; // D6
    LPC_IOCON->PIO3_3  = 0x00; // D7

    // RW pin configured as output and forced to write mode
    LPC_GPIO2->DIR |= (1 << 3);
    LPC_IOCON->PIO2_3 = 0x00;
    LPC_GPIO2->DATA &= ~(1 << 3);
}

/**
 * Sets a specific bit in a GPIO register.
 */
void bitSet(volatile uint32_t *end, uint8_t bit) {
    (*end) |= (1 << bit);
}

/**
 * Clears a specific bit in a GPIO register.
 */
void bitClr(volatile uint32_t *end, uint8_t bit) {
    (*end) &= ~(1 << bit);
}

/**
 * Generates an approximate delay in milliseconds.
 * The delay is based on a busy-wait loop assuming a 12 MHz clock.
 */
void delayms(unsigned int time) {
    for (unsigned int i = 0; i < time * 1000; i++) __NOP();
}


/**
 * Sends a 4-bit nibble to the LCD data lines (D4–D7).
 */
void sendNibble(uint8_t nibble) {


    (nibble & 0x01) ? bitSet(&LCD_D4_PORT->DATA, LCD_D4_PIN) : bitClr(&LCD_D4_PORT->DATA, LCD_D4_PIN);
    (nibble & 0x02) ? bitSet(&LCD_D5_PORT->DATA, LCD_D5_PIN) : bitClr(&LCD_D5_PORT->DATA, LCD_D5_PIN);
    (nibble & 0x04) ? bitSet(&LCD_D6_PORT->DATA, LCD_D6_PIN) : bitClr(&LCD_D6_PORT->DATA, LCD_D6_PIN);
    (nibble & 0x08) ? bitSet(&LCD_D7_PORT->DATA, LCD_D7_PIN) : bitClr(&LCD_D7_PORT->DATA, LCD_D7_PIN);


}


/**
 * Sends a full byte to the LCD using 4-bit mode.
 * The RS pin selects between command and data.
 */
void LCDsendByte(uint8_t data, int command) {


    if (command)
        bitClr(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // command
    else
        bitSet(&LCD_RS_PORT->DATA, LCD_RS_PIN);  // character

    sendNibble(data >> 4);
    pulseEnable();

    sendNibble(data & 0x0F);
    pulseEnable();

  
}


/**
 * Sends a command byte to the LCD.
 */
void LCDcmd(uint8_t cmd) {

    LCDsendByte(cmd, 1);
    delayms(2);
}


/**
 * Sends a single character to the LCD.
 */
void LCDcharacter(const char *character){

	LCDsendByte(*character, 0);

}


/**
 * Writes a text string to the LCD.
 * Automatically jumps to the second line after 16 characters.
 */
void LCDwrite(const char *text) {

	int cont = 0;
    while (*text) {
        if(cont == 16) LCDcmd(0xC0);
        else if(cont == 32) break;
    	LCDsendByte(*text++, 0);
        cont += 1;

    }

}


/**
 * Initializes the LCD in 4-bit mode.
 * Executes the standard initialization sequence.
 */
void LCDinit()
{
    bitClr(&LCD_E_PORT->DATA,  LCD_E_PIN);
    bitClr(&LCD_RS_PORT->DATA, LCD_RS_PIN);

    delayms(100);

    sendNibble(0x03); pulseEnable(); delayms(5);
    sendNibble(0x03); pulseEnable(); delayms(5);
    sendNibble(0x03); pulseEnable(); delayms(5);
    sendNibble(0x02); pulseEnable(); delayms(5);

    LCDcmd(0x28); delayms(5);
    LCDcmd(0x0C); delayms(5);
    LCDcmd(0x06); delayms(5);
    LCDcmd(0x01); delayms(5);

}

/**
 * Generates the enable pulse required by the LCD.
 */
void pulseEnable() {

    bitSet(&LCD_E_PORT->DATA, LCD_E_PIN);
    bitClr(&LCD_E_PORT->DATA, LCD_E_PIN);

}

/**
 * Sets the LCD cursor position based on line and column.
 */
void LCDsetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    // Column limit correction
    if (column > 15) column = 15;

    switch (row) {
        case 0:
            address = 0x00 + column;
            break;
        case 1:
            address = 0x40 + column;
            break;
        default:
            address = 0x00;
            break;
    }

    LCDcmd(0x80 | address);
}

/**
 * Converts a BCD value to an ASCII string.
 */
char* convertBCD2ASCII(char word){

  static char buffer[3];

  // Add ASCII offset for digit conversion
  buffer[0] = ((word >> 4) & 0x0F) + '0';
  buffer[1] = (	word 	   & 0x0F) + '0';
  buffer[2] = '\0';

  return buffer;

}

/**
 * Displays an integer value on the LCD.
 */
void LCDint(int num){

	char buffer[6];
    if (num < 0) {
        num = num * (-1);
        LCDsendByte('-', 1);
    }
    buffer[0] = (num / 10000) % 10 + 48;
    buffer[1] = (num / 1000) % 10 + 48;
    buffer[2] = (num / 100) % 10 + 48;
    buffer[3] = (num / 10) % 10 + 48;
    buffer[4] = (num / 1) % 10 + 48;
    buffer[5] = '\0';

    LCDwrite(buffer);
}
