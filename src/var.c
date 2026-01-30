// -----------------------------------------------------------------------
//  * File: var.c
//      Variable and configuration module. This file centralizes the system
//      state variables, user settings and RTC-related time/date handling.
//      It provides getters and setters to keep access consistent across
//      the application.
//
//  * Authors:
//      Eduardo Castro
//      Glauber Moura
//
//  * Year: 2025/2026
// -----------------------------------------------------------------------

#include "var.h"
#include "i2cRtc.h"

// Variables to be stored
static int state;
static char language;
static int timeVar;
static int alarmLowLevel, alarmHighLevel;
static char hour, minute, second;
static char day, month, year;
static char config_time, config_date;


/**
 * Initializes the main system variables with default values.
 * These defaults define initial state, timing parameters, alarm limits
 * and a starting date/time for the RTC-related logic.
 */
void varInit(void) {

    state = 0;
    timeVar = 1000;
    alarmLowLevel = 256;
    alarmHighLevel = 768;
    hour = 12;
    minute = 30;
    second = 30;
    day = 15;
    month = 6;
    year = 25;

    setTime();
    setDate();
}


/**
 * Returns the current system state value.
 */
int getState(void) {
    return state;
}

/**
 * Updates the current system state value.
 */
void setState(int newState) {
    state = newState;
}


/**
 * Returns the current time variable used as a timing parameter.
 */
int getTimeVar(void) {
    return timeVar;
}

/**
 * Updates the time variable used as a timing parameter.
 */
void setTimeVar(int newTime) {
    timeVar = newTime;
}


/**
 * Returns the current low alarm threshold.
 */
int getAlarmLowLevel(void) {
    return alarmLowLevel;
}

/**
 * Updates the low alarm threshold while keeping it below the high threshold.
 * If the requested value violates the limit, it is clamped accordingly.
 */
void setAlarmLowLevel(int newAlarmLowLevel) {
    alarmLowLevel = getAlarmHighLevel() > newAlarmLowLevel ? newAlarmLowLevel: getAlarmHighLevel()-1;
}


/**
 * Returns the current high alarm threshold.
 */
int getAlarmHighLevel(void) {
    return alarmHighLevel;
}

/**
 * Updates the high alarm threshold while keeping it above the low threshold.
 * If the requested value violates the limit, it is clamped accordingly.
 */
void setAlarmHighLevel(int newAlarmHighLevel) {
    alarmHighLevel = getAlarmLowLevel() < newAlarmHighLevel ? newAlarmHighLevel : getAlarmLowLevel()+1;
}


/**
 * Resets the time fields to their default values.
 * This is useful when discarding a partial configuration.
 */
void flushTime(){

    hour = 12;
    minute = 30;
    second = 30;
}


/**
 * Resets the date fields to their default values.
 * This is useful when discarding a partial configuration.
 */
void flushDate(){

    day = 15;
    month = 6;
    year = 25;
}


/**
 * Returns the current hour value.
 */
char getHour(void){return hour;}

/**
 * Updates the hour value, ensuring it stays within valid bounds.
 */
void setHour(char newHour){
    char realNewHour =  newHour < 24 && 0 <= newHour ? newHour : 0;
    hour = realNewHour;}


/**
 * Returns the current minute value.
 */
char getMinute(void){return minute;}

/**
 * Updates the minute value, ensuring it stays within valid bounds.
 */
void setMinute(char newMinute){
    char realNewminute = newMinute < 60 && 0 <= newMinute ? newMinute : 0;
    minute = realNewminute;}


/**
 * Returns the current second value.
 */
char getSecond(void){return second;}

/**
 * Updates the second value, ensuring it stays within valid bounds.
 */
void setSecond(char newSecond){
    char realNewSecond = newSecond < 60 && 0 <= newSecond ? newSecond : 0;
    second = realNewSecond;}


/**
 * Returns the current day value.
 */
char getDay(void){return day;}

/**
 * Updates the day value with a simple bound check.
 * If the value is invalid, a default day is used.
 */
void setDay(char newDay){
    char realNewDay = newDay < 32 && 1 < newDay? newDay : 1;
    day = realNewDay;
}


/**
 * Returns the current month value.
 */
char getMonth(void){return month;}

/**
 * Updates the month value with a simple bound check.
 * If the value is invalid, a default month is used.
 */
void setMonth(char newMonth){
    char realNewMonth = newMonth < 13 && 1 < newMonth ? newMonth : 1;
    month = realNewMonth;
}


/**
 * Returns the current year value.
 */
char getYear(){return year;}

/**
 * Updates the year value with a simple bound check.
 * If the value is invalid, a default year is used.
 */
void setYear(char newYear){
    char realNewYear = newYear < 100 && newYear >= 0 ? newYear : 25;
    year = realNewYear;
}


/**
 * Converts a decimal number into BCD format.
 * This is used when preparing values to be written into the RTC registers.
 */
uint32_t dec2bcd(uint16_t dec)
{
    uint32_t result = 0;
    int shift = 0;

    while (dec)
    {
        result +=  (dec % 10) << shift;
        dec = dec / 10;
        shift += 4;
    }
    return result;
}


/**
 * Writes the current hour, minute and second values to the RTC.
 * The values are converted to BCD before being sent through the RTC interface.
 */
void setTime()
{
    uint32_t hour_ = dec2bcd(hour);
    uint32_t minute_ = dec2bcd(minute);
    uint32_t second_ = dec2bcd(second);

    config_time = *RTCConfig(0, hour_, minute_, second_);
}


/**
 * Writes the current day, month and year values to the RTC.
 * The values are converted to BCD before being sent through the RTC interface.
 */
void setDate(){

    uint32_t day_ = dec2bcd(day);
    uint32_t month_ = dec2bcd(month);
    uint32_t year_ = dec2bcd(year);

    config_date = *RTCConfig(1, day_, month_, year_);
}


/**
 * Reads the current time registers from the RTC and updates local variables.
 * The masking clears control bits so only the relevant BCD fields remain.
 */
void updateTime(){

    char time[3];
    getRTCData(time, &config_time);
    // Clear the last bit
    second = time[0] & 0x7F;
    minute  = time[1] & 0x7F;
    hour    = time[2] & 0x3F; // Hour up to bit 5

}


/**
 * Reads the current date registers from the RTC and updates local variables.
 */
void updateDate(){
    char data[3];
    getRTCData(data, &config_date);

    day = data[0];
    month = data[1];
    year = data[2];

}


/**
 * Returns the current language selection.
 */
char getLanguage(void){
    return language;
}

/**
 * Updates the language selection.
 * The value is reduced using modulo to keep it within the available options.
 */
void setLanguage(char newLanguage){
    // there are only 2 languages
    // using modulo to avoid invalid values
    language = newLanguage%2;
}
