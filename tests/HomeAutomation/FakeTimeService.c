#include "FakeTimeService.h"

static Time fakeTime;
static WakeUpCallback callback;
static int period;

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

void TimerService_SetPeriodAlarmInSeconds(int seconds, WakeUpCallback cb)
{
    period      = seconds;
    callback    = cb;
}
void TimerService_CancelPeriodAlarmInSeconds(int seconds, WakeUpCallback cb)
{
    if(cb == callback && period == seconds)
    {
        callback    = NULL;
        period      = 0;
    }
}

WakeUpCallback FakeTimeService_GetAlarmCallback(void)
{
    return callback;
}

int FakeTimeService_GetAlarmPeriod(void)
{
    return period;
}