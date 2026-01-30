#ifndef VAR_H
#define	VAR_H

void varInit(void);

int getState(void);
void setState(int newState);
int getTimeVar(void);
void setTimeVar(int newTime);
int getAlarmLowLevel(void);
void setAlarmLowLevel(int newAlarmLevel);
int getAlarmHighLevel(void);
void setAlarmHighLevel(int newAlarmLevel);
void flushTime();
void flushDate();
char getHour(void);
void setHour(char newHour);
char getMinute(void);
void setMinute(char newMinute);
char getSecond(void);
void setSecond(char newSecond);
void setTime();
char getDay();
void setDay(char newDay);
char getMonth();
void setMonth(char newMonth);
char getYear();
void setYear(char newYear);
void setDate();
void updateTime();
void updateDate();
char getLanguage(void);
void setLanguage(char newLanguage);
#endif	/* VAR_H */
