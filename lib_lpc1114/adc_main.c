#include "clocks.h"
#include "adc.h"
#include "LCD.h"
#include "LED.h"

int main(){
    clocks_config();
    ADC_Config();
    ADC_pin_config(0);
    LCD_pin_config();
    LCD_iniciar();
    configureLED();
    apagaLEDS();

    int adc_value;
    while(1){
        LCD_cmd(0x80);
        adc_value = ADC_read();
        LCD_Int(adc_value);
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
        apagaLEDS();
    }
    return 0;
}