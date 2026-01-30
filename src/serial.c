// -----------------------------------------------------------------------
//  * File: serial.c
//      UART communication module. This file handles the configuration of
//      the serial interface and provides basic routines for transmitting
//      and receiving characters through the UART peripheral.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------

#include "serial.h"


/**
 * Configures the UART peripheral for serial communication.
 *
 * This function enables the required clocks, configures the GPIO pins
 * for TX and RX operation, sets the UART clock divider and initializes
 * the UART with 8-bit data length and FIFO enabled.
 */
void configureSerial(){


    // Serial communication
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16); // enable clock for the IOCON block (UM10398, 3.5.14)

    // enable clock for the GPIO module (UM10398, 3.5.14)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);

    // Configure GPIO1[6] and GPIO1[7] as outputs (UM10398, section 12.3.2)
    LPC_GPIO1->DIR |= (1 << TX);
    LPC_GPIO1->DIR |= (1 << RX);
    
    LPC_IOCON->PIO1_7 |= 0x01;              // configure the pin as UART TXD (UM10398, 7.4.41)
    LPC_IOCON->PIO1_6 |= 0x01;              // configure the pin as UART RXD (UM10398, 7.4.41)

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); // enable clock for the UART block (UM10398, 3.5.14)
    LPC_SYSCON->UARTCLKDIV |= 0x9B;         // set UART clock divider to generate 19.2 kbps (UM10398, 3.5.16)
    LPC_UART->FCR |= 0x01;                  // enable FIFO (UM10398, 13.5.6)
    LPC_UART->LCR |= 0x03;                  // set word length to 8 bits (UM10398, 13.5.7)
    LPC_UART->TER |= 0x80;                  // enable transmission (UM10398, 13.5.16)


}


/**
 * Reads a character from the serial interface.
 *
 * The function waits for incoming data for a limited time. If no data
 * is received within this interval, it returns zero. When a character
 * is successfully read, it is echoed back through the UART.
 */
char readSerial(){


    char c;
    unsigned int wait_time = 0;
    // Read the received character (UM10398, 13.5.2)
    // wait until the receive FIFO is not empty
    while (!(LPC_UART->LSR & 0x01)){
        if(wait_time++ >= 4) return 0;
        for (unsigned int i = 0; i < 5 * 1000; i++) __NOP();
    }
    c = LPC_UART->RBR; // read from the receive FIFO (UM10398, 13.5.2)

    // wait until the transmit FIFO is empty before echoing
    while ((LPC_UART->LSR & (1 << 6)) == 0);
    writeSerial(c);
    return c;


}


/**
 * Sends a single character through the serial interface.
 *
 * The function waits until there is space in the transmit FIFO,
 * writes the character and blocks until the transmission is completed.
 */
void writeSerial(char c){


    // Send the character (UM10398, 13.5.2)
    while (!(LPC_UART->LSR & 0x20)); // wait until the transmit FIFO is not full
    // write to the transmit FIFO (UM10398, 13.5.2)
    LPC_UART->THR = c;

    // wait until the transmission is fully completed (UM10398, 13.5.9)
    while ((LPC_UART->LSR & (1 << 6)) == 0);


}
