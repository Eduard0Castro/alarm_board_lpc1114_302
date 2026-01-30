#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "i2c_rtc.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void)
{

    clocksConfig();
    configureLED();
    LCDpinConfig();
    LCDinit();
    LCDcmd(0x0F); // Cursor piscando
    I2CConfig();

    char date[3];
    char time[3];
    // 1 para indicar que é data:
    char config_date = *RTCConfig(1, 0x28, 0x06, 0x25);

    // 0 para indicar que é hora
    char config_time = *RTCConfig(0, 0x23, 0x59, 0x55);

    turnOnLEDs();
    while (1)
    {

        getRTCData(date, &config_date);
        getRTCData(time, &config_time);

        char day = date[0];
        char month = date[1];
        char year = date[2];

        // Clear the last bit
        char second = time[0] & 0x7F;
        char minute  = time[1] & 0x7F;
        char hour    = time[2] & 0x3F; //Hour until bit 5

        delay_ms(400);
        toggleLEDs();

        LCD_set_cursor(0, 0);

        LCDwrite(convertBCD2ASCII(hour));
        LCDwrite(":");
        LCDwrite(convertBCD2ASCII(minute));
        LCDwrite(":");
        LCDwrite(convertBCD2ASCII(second));

        LCD_set_cursor(1, 0);
        LCDwrite(convertBCD2ASCII(day));
        LCDwrite("/");
        LCDwrite(convertBCD2ASCII(month));
        LCDwrite("/");
        LCDwrite(convertBCD2ASCII(year));


    }
    return 0;
}
