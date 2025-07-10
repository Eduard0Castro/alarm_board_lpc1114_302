#ifndef VAR_H
#define	VAR_H

void varInit(void);

char getState(void);
void setState(char newState);
int getTime(void);
void setTime(int newTime);
int getAlarmLowLevel(void);
void setAlarmLowLevel(int newAlarmLevel);
int getAlarmHighLevel(void);
void setAlarmHighLevel(int newAlarmLevel);
void flushTime();
void flushDate();
char getHora(void);
void setHora(char newHora);
char getMinuto(void);
void setMinuto(char newMinuto);
char getSegundo(void);
void setSegundo(char newSegundo);
void setHorario();
char getDia();
void setDia(char newDia);
char getMes();
void setMes(char newMes);
char getAno();
void setAno(char newAno);
void setData();
void updateHorario();
void updateData();
char getLanguage(void);
void setLanguage(char newLanguage);
#endif	/* VAR_H */
