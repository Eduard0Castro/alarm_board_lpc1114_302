#include "LPC11xx.h"
#include "system_LPC11xx.h"
#include "LCD.h"
#include "clocks.h"
#include "LED.h"


int main(void) {

    clocksConfig();

    configureLED();
    turnOffLEDs();

    LCDpinConfig();
    LCDinit();
    LCDcmd(0x0F); 
    LCDwrite("Du, Dudu e Glauber");

    turnOnLEDs();

    while (1){
        toggleLEDs();
        delay_ms(50);
    };
}
