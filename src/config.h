#ifndef CONFIG_H
#define CONFIG_H
#define LOG_MAX_LEN 60
const char* host = "script.google.com";

char ssidAP[] = "AUTOMATION_AP";      
char pwdAP[] = ""; //"12345678"; 


//we leave room for 10 char eTag
//const char funcName[] = "AKfycbxoZduO9st0DHum7ll8LiGjWhNOlgsmvrOdmP0uOrYzz5g_NL4";

#define PLAN_URL_BASE "/macros/s/%s/exec?eTag=%s&cal=%s"
#define LOG_URL_BASE  "/macros/s/%s/exec?log=%d%%3A%d%%3A%d%%3A%d%%3A%s"
//char planUrl[sizeof(PLAN_URL) + 11] = PLAN_URL;

#define MILLIS_BETWEEN_SERVER_POLL 86400000 //24 hours
#define DEFAULT_ADHOC_MIN 5
#endif