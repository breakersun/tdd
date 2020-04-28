#ifndef __LIGHTSCHEDULER_H__
#define __LIGHTSCHEDULER_H__

#include "TimeService.h"

typedef enum
{
    EVERYDAY = 10, WEEKDAY, WEEKEND,
    SUNDAY = 1, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY,
    MAX_EVENTS = 128
} Day;

void LightScheduler_Create(void);
void LightScheduler_Destroy(void);
void LightScheduler_Wakeup(void);
void LightScheduler_ScheduleTurnOn(int id, int day, int minutes);
void LightScheduler_ScheduleTurnOff(int id, int day, int minutes);

#endif