#ifndef _TIME_H_
#define _TIME_H_


#include "sessionTypes.h"

extern "C" void updateNewTime(const char *time);
extern "C" Time * updateTime();
extern "C" unsigned int getMinFromWeekStart();

#endif