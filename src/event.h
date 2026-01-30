#ifndef EVENT_H
#define	EVENT_H
enum{
    EV_ENTER,
    EV_UP,
    EV_DOWN,
    EV_LEFT,
    EV_RIGHT,
    EV_NOEVENT,
	EV_PROTOCOL
};
void eventInit(void);
unsigned int eventRead(void);
char *getProtocolMSG();

#endif	/* EVENT_H */

