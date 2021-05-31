#include "sessionTypes.h"

Time gTime;
unsigned long offsetMillisToWeekStart;

/**
 * Expects a string like <day-of-week 1-7>:hh:mm:ss
 */
void updateNewTime(const char *time) {
    //calculate the offset from last boot time to midnight between Sat and Sun
    int pos = 0;
    int day = getInt(time, pos);
    int hour = getInt(time, pos);
    int min = getInt(time, pos);
    int sec = getInt(time, pos);

    unsigned long millisFromWeekStart = getMillisFromWeekStart(day, hour, min, sec);

    offsetMillisToWeekStart = millis() - millisFromWeekStart;
}
Time * updateTime() {
    unsigned long secondsFromWeekStart = getSecondsFromWeekStart();

    gTime.sec = secondsFromWeekStart % 60;
    secondsFromWeekStart = secondsFromWeekStart/60;

    gTime.min = secondsFromWeekStart % 60;
    secondsFromWeekStart = secondsFromWeekStart/60;
    
    gTime.hour = secondsFromWeekStart % 24;
    secondsFromWeekStart = secondsFromWeekStart/24;

    gTime.day = secondsFromWeekStart + 1;

    return &gTime;
}

void showTime() {
    updateTime();
    SERIAL_(gTime.day);SERIAL_(":");
    SERIAL_(gTime.hour);SERIAL_(":");
    SERIAL_(gTime.min);SERIAL_(":");
    SERIAL_(gTime.sec);SERIAL_ln("");
}



unsigned long getMillisFromWeekStart(int day, int hour, int min, int sec) {
    int minFromWeekStart = getMinFromWeekStart(day, hour, min);
    return (minFromWeekStart*60 + sec)*1000;
}

unsigned long getMinFromWeekStart(int day, int hour, int min) {
    return (day-1)*24*60 + hour*60 + min;
}

unsigned int getMinFromWeekStart() {
    return getSecondsFromWeekStart()/60;
}

unsigned int getSecondsFromWeekStart() {
    unsigned int seconds = (millis() - offsetMillisToWeekStart)/1000;
    return seconds % 604800;  // seconds in a week 
}

