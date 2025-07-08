#include "adc.h"

void ADC_Config(){
    // configurar o pino UART TXD (UM10398, 7.4.41)
    LPC_IOCON->PIO1_7 |= 0x01;
    
    // configurar o pino UART RXD (UM10398, 7.4.41)
    LPC_IOCON->PIO1_6 |= 0x01;

    // habiltiar o FIFO (necessário para operar) (UM10398, 13.5.6)
    LPC_UART->FCR |= 0x01;
    // estipular um tamanho de palavra de 8 bits (UM10398, 13.5.7)
    LPC_UART->LCR |= 0x03;
    // habilitar a transmissão (UM10398, 13.5.16)
    LPC_UART->TER |= 0x80;
    
    // Configurar o AD para amostrar
    // ativar o AD (UM10398, 3.5.36)
    LPC_SYSCON->PDRUNCFG &= ~(1 << 4);
}
void ADC_pin_config(unsigned char pin){
    switch(pin){
        case 0:
            // configurar o pino PIO0_11 (AD0) como entrada analógica (UM10398, 7.4.29)
            LPC_IOCON->R_PIO0_11 = 0x02;
            break;
    }
}
int ADC_read(){
    // iniciar uma converção ( bits START=01) (UM10398, 25.5.1)
    LPC_ADC->CR |= (1 << 24);
    // esperar pelo fim da conversão (bit DONE em 1) (UM10398, 25.5.4)
    while ((LPC_ADC->GDR < 0x7FFFFFFF))
        ;

    // Converter o resultado da conversão presente nos bits 15-6 de GDR em
    // uma sequência de 3 dígitos hexadecimais

    return (LPC_ADC->GDR >> 6) & 0x03FF;
}