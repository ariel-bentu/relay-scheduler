#ifndef __sessionTypes__
#define __sessionTypes__


#define DEBUG true

#ifdef DEBUG
    #define SERIAL_ln(X) Serial.println(X)
    #define SERIAL_(X) Serial.print(X)
    #define SERIAL_f(X, ...) Serial.printf((X), __VA_ARGS__)
    #define SERIALSTART(X) Serial.begin(115200);
#else
    #define SERIALln(X) 
    #define SERIAL_(X)
    #define SERIAL_f(X, ...) 
    #define SERIALSTART(X)
#endif

#define MAX_RELAYS  6
#define MAX_SESSIONS 128
#define CURRENT_VERSION 1240
#define FUNC_MAX_LEN 100

struct relay {
    int ID;
    bool on;
};

struct WiFiConfig {
    char ssid[50];
    char pwd[30];

    char ssidAP[50];
    char pwdAP[30];
};

struct CalendarConfig {
    char name[15];
    char funcName[FUNC_MAX_LEN];
};

struct session {
    int relayID;
    unsigned int minuteInWeek; //minutes since midnight between Sat. and Sun.
    unsigned int durationMin;
    //bool recurrent;
    //bool executed;
};

struct store {
    int version;
    WiFiConfig wifiConfig;
    CalendarConfig calConfig;
    int activeSessions;
    session sessions[MAX_SESSIONS];
};

struct relayOveride {
    bool on;
    unsigned int until;
};

relayOveride gOveride[MAX_RELAYS];
relay gRelays[MAX_RELAYS];

store gStore;

struct Time {
    int day;
    int hour;
    int min;
    int sec;
};
#endif

