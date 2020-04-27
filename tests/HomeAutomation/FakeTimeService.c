#include "FakeTimeService.h"

static Time fakeTime;

void TimeService_Create(void)
{
    fakeTime.minuteOfDay    = TIME_UNKNOWN;
    fakeTime.dayOfWeek      = TIME_UNKNOWN;
}

void TimeService_Destroy(void)
{
    return;
}

void TimeService_GetTime(Time *time)
{
    time->minuteOfDay       = fakeTime.minuteOfDay;
    time->dayOfWeek         = fakeTime.dayOfWeek;
}

void FakeTimeService_SetMinute(int minute)
{
    fakeTime.minuteOfDay    = minute;
}

void FakeTimeService_SetDay(int day)
{
    fakeTime.dayOfWeek      = day;
}