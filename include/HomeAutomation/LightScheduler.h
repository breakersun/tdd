#ifndef __LIGHTSCHEDULER_H__
#define __LIGHTSCHEDULER_H__

#include "TimeService.h"

typedef enum
{
    EVERYDAY = 10, WEEKDAY, WEEKEND,
    SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY,
    MAX_EVENTS = 128
} Day;

enum { LS_OK, LS_TOO_MANY_EVENTS, LS_ID_OUT_OF_BOUNDS };

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);
void LightScheduler_Wakeup(void);
int LightScheduler_ScheduleTurnOn(int id, int day, int minutes);
int LightScheduler_ScheduleTurnOff(int id, int day, int minutes);

#endif