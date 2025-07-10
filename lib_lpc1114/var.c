#include "var.h"
#include "i2c_rtc.h"

//variaveis a serem armazenadas
static char state;
static char language;
static int time;
static int alarmLowLevel, alarmHighLevel;
static char hora, minuto, segundo;
static char dia, mes, ano;
static char config_time, config_date;

void varInit(void) {
    state = 0;
    time = 1000;
    alarmLowLevel = 256;
    alarmHighLevel = 768;
    hora = 12;
    minuto = 30;
    segundo = 30;
    dia = 15;
    mes = 6;
    ano = 25;
}


char getState(void) {
    return state;
}
void setState(char newState) {
    state = newState;
}


int getTime(void) {
    return time;
}
void setTime(int newTime) {
    time = newTime;
}


int getAlarmLowLevel(void) {
    return alarmLowLevel;
}
void setAlarmLowLevel(int newAlarmLowLevel) {
    alarmLowLevel = getAlarmHighLevel() > newAlarmLowLevel ? newAlarmLowLevel: getAlarmHighLevel()-1;
}

int getAlarmHighLevel(void) {
    return alarmHighLevel;
}
void setAlarmHighLevel(int newAlarmHighLevel) {
    alarmHighLevel = getAlarmLowLevel() < newAlarmHighLevel ? newAlarmHighLevel : getAlarmLowLevel()+1;
}

void flushTime(){

    hora = 12;
    minuto = 30;
    segundo = 30;
}

void flushDate(){

    dia = 15;
    mes = 6;
    ano = 25;
}

char getHora(void){return hora;}
void setHora(char newHora){
    char realNewHora =  newHora < 24 && 0 <= newHora ? newHora: 0;
    hora = realNewHora;}

char getMinuto(void){return minuto;}
void setMinuto(char newMinuto){
    char realNewMinuto = newMinuto < 60 && 0 <= newMinuto ? newMinuto:  0;
    minuto = realNewMinuto;}

char getSegundo(void){return segundo;}
void setSegundo(char newSegundo){
    char realNewSegundo = newSegundo < 60 && 0 <= newSegundo ? newSegundo: 0;
    segundo = realNewSegundo;}

char getDia(void){return dia;}
void setDia(char newDia){
    char realNewDia = newDia < 32 && 0< newDia? newDia : 15;
    dia = realNewDia;
}

char getMes(void){return mes;}
void setMes(char newMes){
    char realNewMes = newMes < 13 && 0 < newMes ? newMes : 6;
    mes = realNewMes;
}

char getAno(){return ano;}
void setAno(char newAno){
    char realNewAno = newAno < 100 && newAno >= 0 ? newAno : 25;
    ano = realNewAno;
}

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

void setHorario(){
    uint32_t hora_ = dec2bcd(hora);
    uint32_t minuto_ = dec2bcd(minuto);
    uint32_t segundo_ = dec2bcd(segundo);

    config_time = *RTCConfig(0, hora_, minuto_, segundo_);
}

void setData(){

    uint32_t dia_ = dec2bcd(dia);
    uint32_t mes_ = dec2bcd(mes);
    uint32_t ano_ = dec2bcd(ano);

    config_date = *RTCConfig(1, dia_, mes_, ano_);
}

void updateHorario(){

    char tempo[3];
    getRTCData(tempo, &config_time);
    // Limpo o último bit
    segundo = tempo[0] & 0x7F;
    minuto  = tempo[1] & 0x7F;
    hora    = tempo[2] & 0x3F; //Hora usa até o bit 5

}

void updateData(){
    char data[3];
    getRTCData(data, &config_date);

    dia = data[0];
    mes = data[1];
    ano = data[2];

}
char getLanguage(void){
    return language;
}
void setLanguage(char newLanguage){
    //so tem 2 linguas
    //usando resto pra evitar colocar valor errado
    language = newLanguage%2;
}
