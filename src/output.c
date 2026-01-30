// -----------------------------------------------------------------------
//  * File: output.c
//      Output control module responsible for updating the LCD interface
//      and visual indicators according to the current system state.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------

#include "output.h"
#include "LCD.h"
#include "stateMachine.h"
#include "var.h"
#include "LED.h"
#include "adc.h"

#define NUM_LANGUAGES 2

// Messages with 16 characters
// One message per state (top line only)
static char * msgs[STATE_END][NUM_LANGUAGES] = {
        {"Alterar alarme ", "Change alarm   "},
        {"Alterar tempo  ", "Change timer   "},
        {"Alterar idioma ", "Change language"},
        {"Alterar hora   ", "Change hour    "},
        {"Alterar minuto ", "Change minute  "},
        {"Alterar segundo", "Change second  "},
        {"Horario atual  ", "Current time   "},
        {"Alterar dia    ", "Change day     "},
        {"Alterar mes    ", "Change Month   "},
        {"Alterar ano    ", "Change year    "},
        {"Data atual     ", "Current date   "},
        {"ALERTA ALARME  ", "ALARM WARNING  "}
};

void outputInit(void) {
LCDinit();
}

void outputPrint(int numScreen, int language) {
    LCDcmd(0x80);
    LCDwrite(msgs[numScreen][language]);
    LCDcmd(0xC0);
    turnOffLEDs();

    switch (numScreen){

        case STATE_TIME_VAR:
            LCDint(getTimeVar());
            LCDwrite("           ");
            break;

        case STATE_ALARM:
            LCDint(getAlarmLowLevel());
            getAlarmLevel()?LCDwrite("     >"):LCDwrite("<     ");
            LCDint(getAlarmHighLevel());
            break;

        case STATE_LANGUAGE:
            if (getLanguage() == 0) {
                LCDwrite("Portugues       ");
            }
            if (getLanguage() == 1) {
                LCDwrite("English         ");
            }
            break;

        case STATE_HOUR:
            LCDint(getHour());
            LCDwrite("           ");

            break;

        case STATE_MINUTE:
            LCDint(getMinute());
            LCDwrite("           ");

            break;

        case STATE_SECOND:
            LCDint(getSecond());
            LCDwrite("           ");

            break;

        case STATE_TIME:
            updateTime();
            LCDwrite(convertBCD2ASCII(getHour()));
            LCDwrite(":");
            LCDwrite(convertBCD2ASCII(getMinute()));
            LCDwrite(":");
            LCDwrite(convertBCD2ASCII(getSecond()));

            break;

        case STATE_DAY:
            LCDint(getDay());
            LCDwrite("           ");

            break;

        case STATE_MONTH:
            LCDint(getMonth());
            LCDwrite("           ");

            break;

        case STATE_YEAR:
            LCDint(getYear());
            LCDwrite("           ");

            break;

        case STATE_DATE:
            updateDate();
            LCDwrite(convertBCD2ASCII(getDay()));
            LCDwrite("/");
            LCDwrite(convertBCD2ASCII(getMonth()));
            LCDwrite("/");
            LCDwrite(convertBCD2ASCII(getYear()));

            break;

        case STATE_WARNING:
            turnOnLEDs();
            LCDint(ADCread());
            LCDwrite("            ");

            break;

        default: break;
    }
}
