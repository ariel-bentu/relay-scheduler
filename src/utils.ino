#include "sessionTypes.h"
#include "config.h"
#include <EEPROM.h>

extern store gStore;
extern const int pinsArray[9];

extern HardwareSerial Serial;

void initSerial() {
    
    SERIALSTART(115200)
}


bool deserializePlan(const char* plan, store & intoStore, String & msg) {
    SERIAL_("Sessions: ");
    int pos = 0;
    int numOfSessions = getInt(plan, pos);
    if (numOfSessions >  MAX_SESSIONS) {
        msg = "Exceeds max sessions,max:";
        msg += MAX_SESSIONS;
        msg += " got:";
        msg += numOfSessions;
        SERIAL_(msg);
        
        return false;
    }
    intoStore.activeSessions = numOfSessions;
    SERIAL_(intoStore.activeSessions);
    SERIAL_ln(":");

    for (int i = 0 ; i < intoStore.activeSessions;i++) {
        intoStore.sessions[i].relayID = getInt(plan, pos);
        SERIAL_(" relay:");
        SERIAL_ln(intoStore.sessions[i].relayID);
        SERIAL_(" day:");
        int day = getInt(plan, pos);
        int hour = getInt(plan, pos);
        int min = getInt(plan, pos);
        SERIAL_(day);
        SERIAL_(":");
        SERIAL_(hour);
        SERIAL_(":");
        SERIAL_ln(min);

        intoStore.sessions[i].minuteInWeek = getMinFromWeekStart(day,hour,min);
        SERIAL_(" minInWeek:");
        SERIAL_ln(intoStore.sessions[i].minuteInWeek);
        
        intoStore.sessions[i].durationMin = getInt(plan, pos);
        SERIAL_(" duration:");
        SERIAL_ln(intoStore.sessions[i].durationMin);

        //skip recurrent flag
        getInt(plan, pos);
        //intoStore.sessions[i].executed = false;
        // if (intoStore.sessions[i].recurrent) {
        //     SERIAL_ln(" recu");
        // } else {
        //     SERIAL_ln("");
        // }
        SERIAL_ln("-----");
     }
     msg = plan;
     return true;
}

void initStore() {
    EEPROM.begin(sizeof(store)+1);
    EEPROM.get(0, gStore);
    if (gStore.version != CURRENT_VERSION) {
        //assume garbage from ROM
        memset(&gStore, 0, sizeof(store));
        gStore.version = CURRENT_VERSION;
        strcpy(gStore.wifiConfig.ssidAP, ssidAP);
        strcpy(gStore.wifiConfig.pwdAP, pwdAP);

        SERIAL_ln("Empty store. using defaults");
    }
}

void applyPlan(const char*plan, String & msg) {
    store newStore = gStore;
    if (deserializePlan(plan, newStore, msg)) {
        gStore = newStore;
        saveStore();

        SERIAL_ln("New setup saved");
    }
}

void saveStore() {
    SERIAL_ln("saveStore");
    gStore.version = CURRENT_VERSION;
    EEPROM.put(0, gStore);
    if (EEPROM.commit()) {
        SERIAL_ln(".success");
    }
    
}


int getInt(const char*input, int & pos) {
    //skip white space of :
    while(isspace(*(input+pos)) || *(input+pos) == ':')
        pos++;

    char intStr[11];
    int i = 0;
    while(i<10 && !isspace(*(input+pos)) && *(input+pos) != ':' && *(input+pos) != ',') {
        intStr[i] = *(input+pos);
        pos++;
        i++;
    }
    intStr[i] = '\0';
    // SERIAL_ln(intStr);

    int ret;
    sscanf(intStr, "%d", &ret);
    pos++; //skip the last whitespace
    return ret;
}


void turnOnOffRelay(int ID, bool on) {
    int index = getRelayIndex(ID);
    if (gRelays[index].on != on) {
        SERIAL_("Turn ");
        SERIAL_(on?"on":"off");
        SERIAL_(" relay ");
        SERIAL_ln(ID);
        gRelays[index].on = on;
        //skip the first pin D0
        digitalWrite(pinsArray[index+1], on?HIGH:LOW);
        String logEntry = "Relay%20";
        logEntry.concat(ID);

        logEntry += "%20switched%20";
        logEntry += on?"on":"off";
        log(logEntry.c_str());
    }
}

void log(const char* message) {
    if (strlen(message) > LOG_MAX_LEN) {
        SERIAL_ln ("Log too long");
        return;
    }
    if (strlen(gStore.calConfig.funcName) == 0) {
        SERIAL_ln("Function name not initialized");
    }
    char logUrl[LOG_MAX_LEN + sizeof(LOG_URL_BASE) + FUNC_MAX_LEN + 1];
    Time * t = updateTime();
    sprintf(logUrl, LOG_URL_BASE, gStore.calConfig.funcName, t->day, t->hour, t->min, t->sec, message);
    String result;
    SERIAL_("log sent: ");
    SERIAL_(logUrl);
    if (!getUrl(logUrl, result)) {
        SERIAL_ln(" - Failed!");
    } else {
        SERIAL_ln(" - Success!");
    }

}

int getRelayIndex(int ID) {
    for (int i=0;i<MAX_RELAYS;i++) {
        if (gRelays[i].ID == ID)
            return i;
    }
    return -1;
}

int parsePipeToken(const char* source, int limit) {
    int pos = 0;
    while (pos < limit && *(source + pos) != '|') {
        pos++;
    }
    return pos;
}