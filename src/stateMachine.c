// -----------------------------------------------------------------------
//  * File: stateMachine.c
//      State machine module responsible for handling user and system events
//      and executing state-specific logic for time, date, alarm configuration
//      and warning supervision.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------


#include <stdlib.h>
#include <string.h>
#include "var.h"
#include "stateMachine.h"
#include "event.h"
#include "output.h"
#include "peripherics.h"
#include "adc.h"
#include "LED.h"

unsigned char alarmLevel = 0;

unsigned char event;
char warningFlag;
int adc_value, alarmLow, alarmHigh;


/**
 * The handle functions defined in this module are responsible for managing
 * the behavior of the system in each specific state. Each function interprets
 * the events that can occur while the system is operating in its respective
 * state and applies the corresponding actions, updating configuration values
 * and internal variables as needed.
 *
 * By assigning a dedicated handler to each state, the state machine logic
 * remains organized and easy to follow, with each routine focusing only on
 * the events that are meaningful in that context. These handlers process
 * both user interactions, such as button events, and protocol-based commands
 * received through the serial interface, keeping the control flow clear and
 * maintainable.
 */

// Handle function prototypes:
void alarmHandle();
void timerHandle();
void languageHandle();
void hourHandle();
void minuteHandle();
void secondHandle();
void timeHandle();
void dayHandle();
void monthHandle();
void yearHandle();
void dateHandle();
void warningHandle();


unsigned char getAlarmLevel(){return alarmLevel;}


State SMStates[] = {{STATE_LANGUAGE,    alarmHandle}, //   ALARM_STATE
                    {STATE_LANGUAGE,    timerHandle}, //   TIMER_STATE
                    {STATE_HOUR,     languageHandle}, //LANGUAGE_STATE
                    {STATE_MINUTE,       hourHandle}, //    HOUR_STATE
                    {STATE_SECOND,     minuteHandle}, //  MINUTE_STATE
                    {STATE_TIME,       secondHandle}, //  SECOND_STATE
                    {STATE_DAY,          timeHandle}, //   TIME_HANDLE
                    {STATE_MONTH,         dayHandle}, //     DAY_STATE
                    {STATE_YEAR,        monthHandle}, //   MONTH_STATE
                    {STATE_DATE,         yearHandle}, //    YEAR_STATE
                    {STATE_ALARM, 	     dateHandle}, //    DATE_STATE
};
State warningState = {.next = STATE_ALARM, .handleFunc = warningHandle};


void smInit(void) {

    alarmLow = getAlarmLowLevel();
    alarmHigh = getAlarmHighLevel();
    setState(STATE_ALARM);

}

void smLoop(void) {

    event = eventRead();
    adc_value = ADCread();

    warningState.handleFunc();
    SMStates[getState()].handleFunc();

    if (event == EV_ENTER) {
        setState(SMStates[getState()].next);
    }

    outputPrint(getState(), getLanguage());
}


void warningHandle(){


    if((adc_value < alarmLow || adc_value > alarmHigh) && !warningFlag){
        warningState.next = getState();
        setState(STATE_WARNING);
        warningFlag = 1;
    }

    else if((adc_value > alarmLow && adc_value < alarmHigh) && warningFlag){
        warningFlag = 0;
        setState(warningState.next);
    }

}

void alarmHandle(){

    if (event == EV_UP)
        alarmLevel ? setAlarmHighLevel(getAlarmHighLevel() + 1):
                     setAlarmLowLevel(getAlarmLowLevel() + 1) ;
    else if (event == EV_DOWN)
        alarmLevel ? setAlarmHighLevel(getAlarmHighLevel() - 1):
                     setAlarmLowLevel(getAlarmLowLevel() - 1) ;
    else if(event == EV_RIGHT)alarmLevel = 0;
    else if(event == EV_LEFT)alarmLevel = 1;

    if(event == EV_PROTOCOL){
        alarmLevel ? setAlarmHighLevel(atoi(getProtocolMSG())):
                     setAlarmLowLevel(atoi(getProtocolMSG())) ;
    }

    alarmLow = getAlarmLowLevel();
    alarmHigh = getAlarmHighLevel();

}


void timerHandle(){

    if      (event == EV_RIGHT) setTimeVar(getTimeVar() + 1);
    else if (event == EV_LEFT) setTimeVar(getTimeVar() - 1);
    if      (event == EV_PROTOCOL) setTimeVar(atoi(getProtocolMSG()));
}


void languageHandle(){

    if      (event == EV_RIGHT) setLanguage(getLanguage() + 1);
    else if (event == EV_LEFT) setLanguage(getLanguage() - 1);
    else if (event == EV_ENTER) flushTime();

    if      (event == EV_PROTOCOL){
        char *language = getProtocolMSG();
        if(strcasecmp(language, "EN")==0)setLanguage(1);
        else if(strcasecmp(language, "PT") == 0) setLanguage(0);
    }
}


void hourHandle(){

    if      (event == EV_RIGHT) setHour(getHour()+1);
    else if (event == EV_LEFT) setHour(getHour()-1);
    if      (event == EV_PROTOCOL) setHour(atoi(getProtocolMSG()));


}

void minuteHandle(){

    if      (event == EV_RIGHT) setMinute(getMinute()+ 1);
    else if (event == EV_LEFT) setMinute(getMinute()- 1);
    if      (event == EV_PROTOCOL) setMinute(atoi(getProtocolMSG()));

}


void secondHandle(){


    if      (event ==    EV_RIGHT) setSecond(getSecond()+ 1);
    else if (event ==     EV_LEFT) setSecond(getSecond()- 1);
    else if (event ==    EV_ENTER) setTime();
    else if (event ==       EV_UP) setState(STATE_TIME);

    if      (event == EV_PROTOCOL) setSecond(atoi(getProtocolMSG()));

}

void timeHandle(){

    if (event == EV_ENTER) flushDate();

    if (event == EV_PROTOCOL){
        char * horario = getProtocolMSG();
        char hora[2] = {horario[0], horario[1]};
        char minuto[2] = {horario[2], horario[3]};
        char segundo[2] = {horario[4], horario[5]};
        setHour(atoi(hora));
        setMinute(atoi(minuto));
        setSecond(atoi(segundo));
        setTime();
    }
}


void dayHandle(){

    if      (event == EV_RIGHT) setDay(getDay()+1);
    else if (event == EV_LEFT) setDay(getDay()-1);

    if      (event == EV_PROTOCOL) setDay(atoi(getProtocolMSG()));
}


void monthHandle(){

    if      (event == EV_RIGHT) setMonth(getMonth()+1);
    else if (event == EV_LEFT) setMonth(getMonth()-1);

    if      (event == EV_PROTOCOL) setMonth(atoi(getProtocolMSG()));
}


void yearHandle(){

    if      (event == EV_RIGHT) setYear(getYear()+1);
    else if (event ==  EV_LEFT) setYear(getYear()-1);
    else if (event == EV_ENTER) setDate();
    else if (event ==    EV_UP) setState(SMStates[getState()].next);

    if      (event == EV_PROTOCOL) setYear(atoi(getProtocolMSG()));

}


void dateHandle(){


    if(event == EV_PROTOCOL){
        char * data = getProtocolMSG();
        char dia[2] = {data[0], data[1]};
        char mes[2] = {data[2], data[3]};
        char ano[2] = {data[4], data[5]};
        setDay(atoi(dia));
        setMonth(atoi(mes));
        setYear(atoi(ano));
        setDate();
    }

}
