
#include "sessionTypes.h"
#include "wemosD1Mini.h"

#include "config.h"
#include "utils.h"
#include "time.h"

char eTag[11];
long lastPlanUpdate = 0;
char lastPlanTime[11] = "";

const char *applyEtag(const char *newPlan)
{
    int len = parsePipeToken(newPlan, 10);
    char newEtag[11];
    strncpy(newEtag, newPlan, len);
    newEtag[len] = '\0';

    SERIAL_("Etag: ");
    SERIAL_ln(newEtag);

    if (strlen(newEtag) > 0 && strcmp(newEtag, "-") != 0)
    {
        strcpy(eTag, newEtag);
    }

    //skip the |
    return newPlan + len + 1;
}

const char *applyTime(const char *newPlan)
{
    int len = parsePipeToken(newPlan, 10);
    //day:hh:mm:ss = max 10
    strncpy(lastPlanTime, newPlan, len);
    lastPlanTime[len] = '\0';
    updateNewTime(lastPlanTime);
    updateTime();
    SERIAL_("Time updated: ");
    showTime();

    return newPlan + len + 1;
}

void maybeRefreshPlan()
{
    //todo consider the time
    if (lastPlanUpdate == 0 || (millis() - lastPlanUpdate) > MILLIS_BETWEEN_SERVER_POLL)
    {
        String msg;
        readPlan(msg);
    }
    digitalWrite(LED_BUILTIN, LOW);
}

bool isNewPlan(const char *plan)
{
    if (strlen(plan) == 0)
        return false;

    return strncmp(plan, "-|", 2) != 0;
}

bool readPlan(String &msg)
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    String plan;

    bool res = getPlan(plan, msg);
    if (res)
    {
        const char *p = plan.c_str();
        bool newPlan = isNewPlan(p);
        const char *planNoEtag = applyEtag(p);
        const char *planNoTime = applyTime(planNoEtag);
        if (newPlan)
        {
            applyPlan(planNoTime, msg);
        }
        else
        {
            msg = sprintf("No new plan, %s", plan.c_str());
        }
    }
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
    return res;
}

bool getPlan(String &plan, String &msg)
{
    if (strlen(gStore.calConfig.funcName) == 0 ||
        strlen(gStore.calConfig.name) == 0)
    {
        msg = "Calendar not defined";
        SERIAL_ln(msg);
        return false;
    }

    char planUrl[FUNC_MAX_LEN + sizeof(PLAN_URL_BASE) + 5 + sizeof(eTag)];
    sprintf(planUrl, PLAN_URL_BASE, gStore.calConfig.funcName, eTag, gStore.calConfig.name);
    if (getUrl(planUrl, plan))
    {
        lastPlanUpdate = millis();
        return true;
    }
    return false;
}

String  getLastPlanUpdate()
{
    return lastPlanTime;
}
