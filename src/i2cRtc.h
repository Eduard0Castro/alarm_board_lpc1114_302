#include "LPC11xx.h"
#include "system_LPC11xx.h"

#define K_ADDRESS_MCP7940 0x6F // Endereço em 7 bits 654 3210

void I2CConfig();
void I2CSend(unsigned char address, unsigned char *value, unsigned char size);
void I2CReceive(unsigned char address, unsigned char *value, unsigned char size);
void getRTCData(char *data, char * config);
char * RTCConfig(char, char, char, char);
