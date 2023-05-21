#ifndef _TIME_H_
#define _TIME_H_


#include "sessionTypes.h"

extern "C" void updateNewTime(const char *time);
extern "C" Time * updateTime();
extern "C" unsigned int getMinFromWeekStart();
extern "C" unsigned int dayFromMinFromWeekStart(const unsigned int min);
extern "C" unsigned int hourFromMinFromWeekStart(const unsigned int min);
extern "C" unsigned int minFromMinFromWeekStart(const unsigned int min);
#endif