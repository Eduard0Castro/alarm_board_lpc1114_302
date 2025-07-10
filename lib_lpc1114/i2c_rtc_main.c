#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "i2c_rtc.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void)
{

    clocks_config();
    configureLED();
    LCD_pin_config();
    LCD_iniciar();
    LCD_cmd(0x0F); // Cursor piscando
    I2C_Config();

    char date[3];
    char time[3];
    // 1 para indicar que é data:
    char config_date = *RTCConfig(1, 0x28, 0x06, 0x25);

    // 0 para indicar que é hora
    char config_time = *RTCConfig(0, 0x23, 0x59, 0x55);

    acendeLEDS();
    while (1)
    {

        getRTCData(date, &config_date);
        getRTCData(time, &config_time);

        char dia = date[0];
        char mes = date[1];
        char ano = date[2];

        // Limpo o último bit
        char segundos = time[0] & 0x7F;
        char minutos  = time[1] & 0x7F;
        char horas    = time[2] & 0x3F; //Hora usa até o bit 5

        delay_ms(400);
        toggleLEDS();

        LCD_set_cursor(0, 0);

        LCD_escreve(convertBCD_ASCII(horas));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(minutos));
        LCD_escreve(":");
        LCD_escreve(convertBCD_ASCII(segundos));

        LCD_set_cursor(1, 0);
        LCD_escreve(convertBCD_ASCII(dia));
        LCD_escreve("/");
        LCD_escreve(convertBCD_ASCII(mes));
        LCD_escreve("/");
        LCD_escreve(convertBCD_ASCII(ano));


    }
    return 0;
}
