#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "LCD.h"
#include "clocks.h"
#include "serial.h"
#include "LED.h"
#include "event.h"
#include "var.h"
#include "stateMachine.h"
#include "output.h"
#include "i2c_rtc.h"
#include "adc.h"


int main(void)
{

    //  init das bibliotecas
    clocks_config();
    configureLED();
    apagaLEDS();
    configureSerial();
    LCD_pin_config();
    outputInit();
    eventInit();
    varInit();
    I2C_Config();
    ADC_Config();
    ADC_pin_config(0);


    for (;;) {
        //state machine:
        smLoop();
    }
    return 0;
}
