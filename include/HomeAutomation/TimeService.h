#ifndef __TIMESERVICE_H__
#define __TIMESERVICE_H__

typedef struct Time
{
    int minuteOfDay;
    int dayOfWeek;
} Time;

typedef void (*WakeUpCallback)(void);

void TimeService_Create(void);
void TimeService_Destroy(void);
void TimeService_GetTime(Time *time);
void TimerService_SetPeriodAlarmInSeconds(int seconds, WakeUpCallback callback);
void TimerService_CancelPeriodAlarmInSeconds(int seconds, WakeUpCallback callback);

#endif