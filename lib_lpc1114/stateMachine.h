#ifndef STATEMACHINE_H
#define	STATEMACHINE_H


//estados da maquina de Estados

enum {
    STATE_ALARME,
    STATE_TEMPO,
    STATE_IDIOMA,
    STATE_HORA,
    STATE_MINUTO,
    STATE_SEGUNDO,
    STATE_HORARIO,
    STATE_DIA,
    STATE_MES,
    STATE_ANO,
    STATE_DATA,
    STATE_ALERTA,
    STATE_FIM
};


void smInit(void);
void smLoop(void);
unsigned char getAlarmLevel();


#endif	/* STATEMACHINE_H */

