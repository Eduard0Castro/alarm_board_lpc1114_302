#include "LPC11xx.h"
#include "system_LPC11xx.h"

// Pins definition
#define LCD_RS_PORT LPC_GPIO3
#define LCD_E_PORT  LPC_GPIO1
#define LCD_D4_PORT LPC_GPIO1
#define LCD_D5_PORT LPC_GPIO3
#define LCD_D6_PORT LPC_GPIO1
#define LCD_D7_PORT LPC_GPIO3

#define LCD_RS_PIN  1  // PIO3_1
#define LCD_E_PIN   4  // PIO1_4
#define LCD_D4_PIN 11  // PIO1_11
#define LCD_D5_PIN  2  // PIO3_2
#define LCD_D6_PIN  5  // PIO1_5
#define LCD_D7_PIN  3  // PIO3_3


void LCDpinConfig();
void delayms(unsigned int time);
void bitSet(volatile uint32_t *end, uint8_t bit);
void bitClr(volatile uint32_t *end, uint8_t bit);
void sendNibble(uint8_t nibble);
void LCDsendByte(uint8_t data, int command);
void LCDinit(void);
void LCDwrite(const char *text);
void LCDcmd(uint8_t cmd);
void LCDcharacter(const char *character);
void pulseEnable();
void LCDsetCursor(uint8_t row, uint8_t column);
char * convertBCD2ASCII(char);
void LCDint(int);
