#include "clocks.h"
#include "adc.h"
#include "LCD.h"

int main(){
    clocks_config();
    ADC_Config();
    ADC_pin_config(0);
    LCD_pin_config();
    LCD_iniciar();

    int adc_value;
    while(1){
        LCD_cmd(0x80);
        adc_value = ADC_read();
        LCD_Int(adc_value);
    }
    return 0;
}