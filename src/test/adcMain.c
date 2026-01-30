#include "clocks.h"
#include "adc.h"
#include "LCD.h"
#include "LED.h"

int main(){
    clocks_config();
    ADCconfig();
    ADCpinConfig(0);
    LCDpinConfig();
    LCDinit();

    configureLED();
    turnOffLEDs();

    int adc_value;
    while(1){
        LCDcmd(0x80);
        adc_value = ADC_read();
        LCDint(adc_value);

        if (adc_value < 256) {
            toggleLED(1);
            for(int i = 0; i < 0x3FFF; i++);
        }
        else if (adc_value < 512) {
            toggleLED(2);
            for(int i = 0; i < 0x3FFF; i++);
        }
        else if (adc_value < 768) {
            toggleLED(3);
            for(int i = 0; i < 0x3FFF; i++);
        }
        else {
            toggleLED(4);
            for(int i = 0; i < 0x3FFF; i++);
        }

        turnOffLEDs();
    }
    return 0;
}