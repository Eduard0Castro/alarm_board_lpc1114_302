#ifndef STATEMACHINE_H
#define	STATEMACHINE_H


typedef void (*stateHandle)(void);


enum {
    STATE_ALARM,
    STATE_TIME_VAR,
    STATE_LANGUAGE,
    STATE_HOUR,
    STATE_MINUTE,
    STATE_SECOND,
    STATE_TIME,
    STATE_DAY,
    STATE_MONTH,
    STATE_YEAR,
    STATE_DATE,
    STATE_WARNING,
    STATE_END
};

typedef struct {

	unsigned char next;
	stateHandle handleFunc;

}State;


void smInit(void);
void smLoop(void);
unsigned char getAlarmLevel();


#endif	/* STATEMACHINE_H */

