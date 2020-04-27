#ifndef __FAKETIMESERVICE_H__
#define __FAKETIMESERVICE_H__

#include "TimeService.h"

enum {TIME_UNKNOWN = -1};

void FakeTimeService_SetMinute(int minute);
void FakeTimeService_SetDay(int day);
#endif