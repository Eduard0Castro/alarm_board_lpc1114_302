// -----------------------------------------------------------------------
//  * File: i2cRtc.c
//      Library responsible for configuring and handling I2C communication
//      with the RTC device. This module provides low-level routines for
//      transmitting and receiving data over I2C and higher-level helpers
//      for configuring and reading time and date information from the RTC.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------

#include "i2cRtc.h"


/**
 * This function configures the I2C peripheral to operate in standard mode.
 * It sets the appropriate pin functions for SCL and SDA, ensures that the
 * I2C block is not held in reset and configures the clock divider to achieve
 * a bus frequency of approximately 100 kHz. After configuration, the I2C
 * peripheral is enabled to operate as a master device.
 */
void I2CConfig(){


    // Configure the pin as SCL in standard I2C mode (I2CMODE = 00) (UM10398, 7.4.11)
    LPC_IOCON->PIO0_4 = 0x01;
    // Configure the pin as SDA in standard I2C mode (I2CMODE = 00) (UM10398, 7.4.12)
    LPC_IOCON->PIO0_5 = 0x01;

    // Ensure that the I2C peripheral is not held in reset (UM10398, 3.5.2)
    LPC_SYSCON->PRESETCTRL |= (1 << 1);

    // Set the I2C clock to 100 kHz (UM10398, 15.7.5.1)
    LPC_I2C->SCLH = 240;
    LPC_I2C->SCLL = 240;

    // Enable the I2C peripheral as master (UM10398, 15.7.1 / 15.8.1)
    LPC_I2C->CONSET |= (1 << 6);


}


/**
 * This function transmits one or more bytes to an I2C slave device.
 * It generates a START condition, sends the slave address with the
 * write bit set and sequentially transmits the requested number of
 * data bytes, checking for acknowledgment after each transmission.
 * The communication is finalized by generating a STOP condition.
 */
void I2CSend(unsigned char address, unsigned char *value, unsigned char size){


    unsigned char c;
    // Request the START condition (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 5);

    // Wait for the completion of the START condition (SI bit, UM10398, 15.7.1)
    while (LPC_I2C->STAT != 0x08);

    // Send the slave address with write specification (R/W = 0) (UM10398, 15.7.3)
    LPC_I2C->DAT = address << 1;
    
    // Clear SI and START flags (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (5 << 3);
    
    // Wait for state 0x18 (ACK received) (UM10398, 15.10.1, Table 236)
    while (LPC_I2C->STAT != 0x18);

    // Send the requested data bytes
    for (c = 0; c < size; c++)
    {
        // Transmit the data byte (UM10398, 15.7.3)
        LPC_I2C->DAT = value[c];
        // Clear SI to transmit the byte and wait for ACK (UM10398, 15.7.6 and Table 236)
        LPC_I2C->CONCLR = (1 << 3);
        // Wait for state 0x28 (ACK received) (UM10398, 15.10.1, Table 236)
        while (LPC_I2C->STAT != 0x28);
    }
    // Request the STOP condition (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 4);
    // Clear the SI bit (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (1 << 3);


}


/**
 * This function receives one or more bytes from an I2C slave device.
 * It generates a START condition, sends the slave address with the
 * read bit set and receives the requested number of bytes. An ACK
 * is generated for all bytes except the last one, for which a NACK
 * is generated, followed by a STOP condition.
 */
void I2CReceive(unsigned char address, unsigned char *value, unsigned char size){


    unsigned char c;
    // Request the START condition (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 5);
    // Wait for the completion of the START condition (SI bit, UM10398, 15.7.1)
    while (LPC_I2C->STAT != 0x08);

    // Send the slave address with read specification (R/W = 1) (UM10398, 15.7.3)
    LPC_I2C->DAT = (address << 1) + 1;
    // Clear SI and START flags (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (5 << 3);
    // Wait for address transmission and ACK reception (state 0x40) (UM10398, 15.10.1, Table 237)
    while (LPC_I2C->STAT != 0x40);
    
    // Receive the requested data bytes
    for (c = 0; c < size; c++)
    {
        // When receiving the last byte, a NACK must be generated;
        // otherwise, an ACK is generated
        
        // Clear SI, START and AA bits to generate a NACK (UM10398, 15.7.6 and Table 237)
        LPC_I2C->CONCLR = 0x2C;
        if (c != (size - 1))
        {
            // Set AA to generate an ACK (UM10398, 15.7.1 and Table 237)
            LPC_I2C->CONSET = 0x4;
        }

        // Wait for byte reception (state must be 0x50 or 0x58) (UM10398, 15.10.2, Table 237)
        while ((LPC_I2C->STAT != 0x50) && (LPC_I2C->STAT != 0x58));

        // Read and store the received byte
        value[c] = LPC_I2C->DAT;
    }
    // Request the STOP condition (UM10398, 15.7.1)
    LPC_I2C->CONSET = (1 << 4);
    // Clear the SI bit (UM10398, 15.7.6)
    LPC_I2C->CONCLR = (1 << 3);


}


/**
 * This function retrieves time-related data from the RTC device.
 * It first signals a write operation to select the desired register
 * addresses and then performs a read operation to retrieve the
 * corresponding data bytes from the RTC.
 */
void getRTCData(char * data, char * config){


	// Signal the operation to be performed on the previously specified addresses
	I2CSend(K_ADDRESS_MCP7940,  (unsigned char*)config, 1);

	// Receive the contents of addresses 0x00, 0x01 and 0x02 (seconds, minutes and hours)
	I2CReceive(K_ADDRESS_MCP7940,  (unsigned char*)data, 3);


}


/**
 * This function prepares and applies configuration data to the RTC device.
 * Depending on the selected mode, it configures either date-related or
 * time-related registers and transmits the corresponding values to the RTC.
 * The configuration buffer is stored statically and returned for further use.
 */
char * RTCConfig(char date, char first, char second, char third){


    static char config[6];
    if(date) {
    	config[0] = 0x04;
    	config[1] = first;
    	config[2] = 0x05;
    	config[3] = second;
    	config[4] = 0x06;
    	config[5] = third;
    }

    else {
        config[0] = 0x00;            // seconds address
        config[1] = 0x80 | third;    // seconds with ST bit enabled
        config[2] = 0x01;            // minutes address
        config[3] = second;
        config[4] = 0x02;            // hours address
        config[5] = first;
    }

     // Signal write operations starting from the selected register addresses

    // Write config[1] to config[0]
    I2CSend(K_ADDRESS_MCP7940, (unsigned char*)&config[0], 2);
    // Write config[3] to config[2]
    I2CSend(K_ADDRESS_MCP7940, (unsigned char*)&config[2], 2);
    // Write config[5] to config[4]
    I2CSend(K_ADDRESS_MCP7940, (unsigned char*)&config[4], 2);

    return config;


}
