#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "stateMachine.h"
#include "event.h"
#include "output.h"
#include "peripherics.h"
#include "adc.h"

unsigned char alarmLevel = 0;
int previous_state;
char alerta_flag;

unsigned char getAlarmLevel(){return alarmLevel;}

void smInit(void) {
    setState(STATE_TEMPO);
}

void smLoop(void) {
    unsigned char evento;
    int adc_value, alarmLow, alarmHigh;
//
//    //maquina de estados
    evento = eventRead();
    adc_value = ADC_read();

    alarmLow = getAlarmLowLevel();
    alarmHigh = getAlarmHighLevel();

    if((adc_value < alarmLow || adc_value > alarmHigh) && !alerta_flag){
        previous_state = getState();
        setState(STATE_ALERTA);
        alerta_flag = 1;
    }

    else if((adc_value > alarmLow && adc_value < alarmHigh) && alerta_flag){
        alerta_flag = 0;
        setState(previous_state);
    }

    switch (getState()) {

        case STATE_ALARME:
            //execucao de atividade
            if (evento == EV_UP) {
                alarmLevel ? setAlarmHighLevel(getAlarmHighLevel() + 1):
                             setAlarmLowLevel(getAlarmLowLevel() + 1) ;
            }
            if (evento == EV_DOWN) {
                alarmLevel ? setAlarmHighLevel(getAlarmHighLevel() - 1):
                             setAlarmLowLevel(getAlarmLowLevel() - 1) ;            }

            if(evento == EV_RIGHT)alarmLevel = 0;
            if(evento == EV_LEFT)alarmLevel = 1;
            if(evento == EV_PROTOCOL){
                alarmLevel ? setAlarmHighLevel(atoi(getProtocolMSG())):
                             setAlarmLowLevel(atoi(getProtocolMSG())) ;
            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_IDIOMA);
            }

            break;

        case STATE_TEMPO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setTime(getTime() + 1);
            }
            if (evento == EV_LEFT) {
                setTime(getTime() - 1);
            }

            if(evento == EV_PROTOCOL){
                setTime(atoi(getProtocolMSG()));
            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_IDIOMA);
            }
            break;

        case STATE_IDIOMA:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setLanguage(getLanguage() + 1);
            }
            if (evento == EV_LEFT) {
                setLanguage(getLanguage() - 1);
            }

            if(evento == EV_PROTOCOL){
                char *language = getProtocolMSG();
                if(strcasecmp(language, "EN")==0)setLanguage(1);
                else if(strcasecmp(language, "PT") == 0) setLanguage(0);
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_HORA);
                flushTime();
            }
            break;

        case STATE_HORA:
            //execucao de atividade
            if (evento == EV_RIGHT) {
                setHora(getHora()+1);
            }
            if (evento == EV_LEFT) {
                setHora(getHora()-1);
            }
            if(evento == EV_PROTOCOL){
                setHora(atoi(getProtocolMSG()));
            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_MINUTO);
            }
            break;

        case STATE_MINUTO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setMinuto(getMinuto()+ 1);
            }
            if (evento == EV_LEFT) {
                setMinuto(getMinuto()- 1);
            }
            if(evento == EV_PROTOCOL){
                setMinuto(atoi(getProtocolMSG()));

            }
            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setState(STATE_SEGUNDO);
            }
            break;

        case STATE_SEGUNDO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setSegundo(getSegundo()+ 1);
            }
            if (evento == EV_LEFT) {
                setSegundo(getSegundo()- 1);
            }
            if(evento == EV_PROTOCOL){
                setSegundo(atoi(getProtocolMSG()));
            }

            //gestao da maquina de estado
            if (evento == EV_ENTER) {
                setHorario();
                setState(STATE_HORARIO);
            }

            if(evento == EV_UP) setState(STATE_HORARIO);

            break;

        case STATE_HORARIO:

            if (evento == EV_ENTER) {
                flushDate();
                setState(STATE_DIA);

            }
            if(evento == EV_PROTOCOL){
                char * horario = getProtocolMSG();
                char hora[2] = {horario[0], horario[1]};
                char minuto[2] = {horario[2], horario[3]};
                char segundo[2] = {horario[4], horario[5]};
                setHora(atoi(hora));
                setMinuto(atoi(minuto));
                setSegundo(atoi(segundo));
                setHorario();

            }

            break;

        case STATE_DIA:
            //execucao de atividade
            if (evento == EV_RIGHT) {
                setDia(getDia()+1);
            }
            if (evento == EV_LEFT) {
                setDia(getDia()-1);
            }
            if(evento == EV_PROTOCOL){
                setDia(atoi(getProtocolMSG()));
            }
            if (evento == EV_ENTER) {
                setState(STATE_MES);
            }

            break;

        case STATE_MES:
            //execucao de atividade
            if (evento == EV_RIGHT) {
                setMes(getMes()+1);
            }
            if (evento == EV_LEFT) {
                setMes(getMes()-1);
            }
            if(evento == EV_PROTOCOL){
                setMes(atoi(getProtocolMSG()));
            }
            if (evento == EV_ENTER) {
                setState(STATE_ANO);
            }

            break;

        case STATE_ANO:

            //execucao de atividade
            if (evento == EV_RIGHT) {
                setAno(getAno()+1);
            }
            if (evento == EV_LEFT) {
                setAno(getAno()-1);
            }
            if(evento == EV_PROTOCOL){
                setAno(atoi(getProtocolMSG()));
            }
            if (evento == EV_ENTER) {
                setData();
                setState(STATE_DATA);
            }
            if(evento == EV_UP) setState(STATE_DATA);


            break;

        case STATE_DATA:
            if (evento == EV_ENTER) {
                setState(STATE_ALARME);
            }
            if(evento == EV_PROTOCOL){
                char * data = getProtocolMSG();
                char dia[2] = {data[0], data[1]};
                char mes[2] = {data[2], data[3]};
                char ano[2] = {data[4], data[5]};
                setDia(atoi(dia));
                setMes(atoi(mes));
                setAno(atoi(ano));
                setData();
            }

            break;

        default: break;

    }
    outputPrint(getState(), getLanguage());
}
