#include "LPC11xx.h" 			// LPC11xx Peripheral Registers
#include "system_LPC11xx.h"

void clock_gpio(){
    // habilitar o clock para o modulo GPIO (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
}
void clock_iocon(){
    // habilitar o clock para o bloco IOCON (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
}
void clock_i2c(){
	// habilitar o clock para o bloco I2C (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);
}
void clock_uart(){
	// habilitar o clock para o bloco UART (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);
	// estipular o clock do módulo UART (divisor de 0x9B) para gerar BR de 19.2K (UM10398, 3.5.16)
	LPC_SYSCON->UARTCLKDIV |= 0x9B;
}
void clock_adc(){
	// habilitar o clock para o bloco ADC (UM10398, 3.5.14)
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 13);
}
void clocks_config(){
	clock_gpio();
	clock_iocon();
	clock_i2c();
	clock_uart();
	clock_adc();
}

