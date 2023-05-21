#define HTTP_DEBUG

#include "sessionTypes.h"
//#include "wemosD1Mini.h"
#include "config.h"
#include "utils.h"
#include "http.h"
#include "plan.h"
#include "time.h"
#include "wifi.h"

//I had to change :
// void WiFiClientSecure::_clearAuthenticationSettings() {
//  _use_insecure = true;
#define PIN0 0
#define PIN1 32 //#6 from the top-left
#define PIN2 33
#define PIN3 25
#define PIN4 26
#define PIN5 27
#define PIN6 14
#define PIN7 12
#define PIN8 13

const int pinsArray[9] = {
    PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7, PIN8
    // WEMOS_PIN_D0, WEMOS_PIN_D1, WEMOS_PIN_D2, 
    // WEMOS_PIN_D3, WEMOS_PIN_D4, WEMOS_PIN_D5, 
    // WEMOS_PIN_D6, WEMOS_PIN_D7, WEMOS_PIN_D8
};


void initRelays()
{
    for (int i = 0; i < MAX_RELAYS; i++)
    {
        pinMode(pinsArray[i + 1], OUTPUT);
        //turn off all relays
        digitalWrite(pinsArray[i + 1], LOW);
        gRelays[i].ID = i + 1;
        gRelays[i].on = false;
    }
}


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    initSerial();
    delay(10);


    initStore();

    initWifi();

    // //open an Access Point for local control and config
    initServerAP();
    updateNewTime("1:0:0:0:0"); //default to start of week
    initRelays();
    SERIAL_ln("3");
    String msg;
    readPlan(msg);
    SERIAL_ln("4");
    log("Device started");
    for (int j = 0; j < MAX_RELAYS; j++) {
        gOveride[j].until = 0;
    }
}


void loop()
{
    Time *t = updateTime();
    if (t->sec == 0)
    {
        showTime();
    }
    maybeRefreshPlan();

    unsigned int timeMinSinceWeekStart = getMinFromWeekStart();
    if (t->sec == 0)
    {
        SERIAL_("Min. from week start:");
        SERIAL_ln(timeMinSinceWeekStart);
    }
    bool relays[MAX_RELAYS];
    for (int j = 0; j < MAX_RELAYS; j++)
        relays[j] = false;

    for (int i = 0; i < gStore.activeSessions; i++)
    {
        bool onOff = gStore.sessions[i].minuteInWeek <= timeMinSinceWeekStart &&
                     (gStore.sessions[i].minuteInWeek + gStore.sessions[i].durationMin) > timeMinSinceWeekStart;

        if (onOff)
        {
            relays[gStore.sessions[i].relayID - 1] = true;
        }
    }

    for (int j = 0; j < MAX_RELAYS; j++)
    {
        if (gOveride[j].until >= timeMinSinceWeekStart && timeMinSinceWeekStart > 0)
        {
            relays[j] = gOveride[j].on;
        }
        else
        {
            gOveride[j].until = 0; 
        }
    }
    for (int j = 0; j < MAX_RELAYS; j++)
        turnOnOffRelay(j + 1, relays[j]);

    loopAP(); 

    delay(1000);
}