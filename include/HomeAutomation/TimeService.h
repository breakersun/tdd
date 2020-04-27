#ifndef __TIMESERVICE_H__
#define __TIMESERVICE_H__

typedef struct Time
{
    int minuteOfDay;
    int dayOfWeek;
} Time;

void TimeService_Create(void);
void TimeService_Destroy(void);
void TimeService_GetTime(Time *time);

#endif