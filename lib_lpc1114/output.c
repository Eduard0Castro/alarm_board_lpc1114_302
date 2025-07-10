#include "output.h"
#include "LCD.h"
#include "stateMachine.h"
#include "var.h"
#include "LED.h"
#include "adc.h"

#define NUM_IDIOMAS 2

//msgs com 16 caracteres
//1 msg por estado (apenas linha de cima)
static char * msgs[STATE_FIM][NUM_IDIOMAS] = {
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
LCD_iniciar();
}

void outputPrint(int numTela, int idioma) {
    LCD_cmd(0x80);
    LCD_escreve(msgs[numTela][idioma]);
    LCD_cmd(0xC0);
    apagaLEDS();

    switch (numTela){

        case STATE_TEMPO:
            LCD_Int(getTime());
            LCD_escreve("           ");//para apagar os textos depois do numero
            break;

        case STATE_ALARME:
            LCD_Int(getAlarmLowLevel());
            getAlarmLevel()?LCD_escreve("     >"):LCD_escreve("<     ");
            LCD_Int(getAlarmHighLevel());
            break;

        case STATE_IDIOMA:
            if (getLanguage() == 0) {
                LCD_escreve("Portugues       ");
            }
            if (getLanguage() == 1) {
                LCD_escreve("English         ");
            }
            break;

        case STATE_HORA:
            LCD_Int(getHora());
            LCD_escreve("           ");

            break;

        case STATE_MINUTO:
            LCD_Int(getMinuto());
            LCD_escreve("           ");

            break;

        case STATE_SEGUNDO:
            LCD_Int(getSegundo());
            LCD_escreve("           ");

            break;

        case STATE_HORARIO:
            updateHorario();
            LCD_escreve(convertBCD_ASCII(getHora()));
            LCD_escreve(":");
            LCD_escreve(convertBCD_ASCII(getMinuto()));
            LCD_escreve(":");
            LCD_escreve(convertBCD_ASCII(getSegundo()));

            break;

        case STATE_DIA:
            LCD_Int(getDia());
            LCD_escreve("           ");

            break;

        case STATE_MES:
            LCD_Int(getMes());
            LCD_escreve("           ");

            break;

        case STATE_ANO:
            LCD_Int(getAno());
            LCD_escreve("           ");

            break;

        case STATE_DATA:
            updateData();
            LCD_escreve(convertBCD_ASCII(getDia()));
            LCD_escreve("/");
            LCD_escreve(convertBCD_ASCII(getMes()));
            LCD_escreve("/");
            LCD_escreve(convertBCD_ASCII(getAno()));

            break;

        case STATE_ALERTA:
            acendeLEDS();
            LCD_Int(ADC_read());
            LCD_escreve("            ");

            break;

        default: break;
    }
    }

