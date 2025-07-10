#include "peripherics.h"
#include "event.h"
#include "serial.h"

static unsigned char key_ant;
static char protocol_msg[16];
static uint8_t protocol_index;
static char protocol_flag;

void eventInit(void) {
    configureButtons();
    key_ant = 0;
    protocol_flag = 0;
    protocol_index = 0;

}

unsigned int eventRead(void) {
    char key, data;
    int ev = EV_NOEVENT;
    key = readButtons();
    if (key != key_ant) {
        if (key == 'r') ev = EV_RIGHT;
        if (key == 'l') ev =  EV_LEFT;
        if (key == 's') ev = EV_ENTER;
        if (key == 'u') ev =    EV_UP;
        if (key == 'd') ev =  EV_DOWN;
    }

    data = readSerial();
    if(data != '[' && !protocol_flag){
        if (data == 'r') ev = EV_RIGHT;
        if (data == 'l') ev =  EV_LEFT;
        if (data == 's') ev = EV_ENTER;
        if (data == 'u') ev =    EV_UP;
        if (data == 'd') ev =  EV_DOWN;
    }

    else {

        protocol_flag = 1;
        if(data == ']'){
            protocol_flag = 0;
            protocol_msg[protocol_index] = '\0';
            protocol_index = 0;
            return EV_PROTOCOL;
        }

        else if(data!='['){
            if (data != '\n' && data != '\0' && data != '\t') {
                if (protocol_index < sizeof(protocol_msg) - 1) {
                    protocol_msg[protocol_index++] = data;
                }
            }
        }
    }

    key_ant = key;
    return ev;
}

char *getProtocolMSG(){return protocol_msg;}
