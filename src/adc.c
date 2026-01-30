#include "adc.h"

/**
 * This function configures the core peripherals required for ADC operation.
 * It sets the UART TXD and RXD pin functions according to the reference manual,
 * enables the UART FIFO, configures the word length to 8 bits and activates
 * transmission. At the end of the configuration, the ADC peripheral is powered
 * up by clearing the corresponding bit in the PDRUNCFG register so that analog
 * conversions can be performed.
 */
void ADCconfig(){

    // Configure the UART TXD pin function according to the LPC reference manual
    LPC_IOCON->PIO1_7 |= 0x01;
    
    // Configure the UART RXD pin function according to the LPC reference manual
    LPC_IOCON->PIO1_6 |= 0x01;

    // Enable the UART FIFO, which is required for proper UART operation
    LPC_UART->FCR |= 0x01;
    // Set the UART word length to 8 bits
    LPC_UART->LCR |= 0x03;
    // Enable UART transmission
    LPC_UART->TER |= 0x80;
    
    // Power up the ADC peripheral so that sampling and conversions can occur
    LPC_SYSCON->PDRUNCFG &= ~(1 << 4);

}

/**
 * This function configures a microcontroller pin to operate as an analog input
 * for the ADC. The selected pin is configured through the IOCON register so
 * that its digital functionality is disabled and the analog input path is
 * enabled, allowing correct sampling by the ADC peripheral.
 */
void ADCpinConfig(unsigned char pin){

    switch(pin){
        case 0:
            // Configure the PIO0_11 pin to operate as ADC channel AD0 in analog mode
            LPC_IOCON->R_PIO0_11 = 0x02;
            break;
    }

}

/**
 * This function starts an analog-to-digital conversion and blocks execution
 * until the conversion is completed. After the DONE flag is set in the Global
 * Data Register, the function extracts the 10-bit conversion result from bits
 * 15 to 6 of the register and returns the digital value corresponding to the
 * sampled analog signal.
 */
int ADCread(){

    // Start an ADC conversion by setting the START bits in the control register
    LPC_ADC->CR |= (1 << 24);
    // Wait until the conversion is finished by monitoring the DONE flag in the
    // global data register
    while ((LPC_ADC->GDR < 0x7FFFFFFF))
        ;

    // Extract and return the 10-bit ADC conversion result from the global data register

    return (LPC_ADC->GDR >> 6) & 0x03FF;

}
