#include "LightScheduler.h"
#include "LightController.h"
#include "common.h"

enum
{
    UNUSED = -1,
    TURN_OFF, TURN_ON,
};

typedef struct
{
    int id;
    int event;
    int minuteOfDay;
    int dayOfWeek;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

void LightScheduler_Create(void)
{
    for(int i = 0; i < MAX_EVENTS; i++)
        scheduledEvents[i].id = UNUSED;

    TimerService_SetPeriodAlarmInSeconds(60, LightScheduler_Wakeup);
}

void LightScheduler_Destroy(void)
{
    TimerService_CancelPeriodAlarmInSeconds(60, LightScheduler_Wakeup);
}

static void operateLight(ScheduledLightEvent *lightevent)
{
    if(lightevent->event == TURN_ON)
        LightController_On(lightevent->id);
    else if(lightevent->event == TURN_OFF)
        LightController_Off(lightevent->id);
}

static int DoesLightRespondToday(Time *time, int reactionDay)
{
    int today = time->dayOfWeek;

    if(reactionDay == EVERYDAY)
        return TRUE;
    if(reactionDay == today)
        return TRUE;
    if(reactionDay == WEEKEND && (today == SATURDAY || today == SUNDAY))
        return TRUE;
    if(reactionDay == WEEKDAY && (today >= MONDAY && today <= FRIDAY))
        return TRUE;
    return FALSE;
}

static void processEventDueNow(Time *time, ScheduledLightEvent *lightevent)
{
    if(lightevent->id == UNUSED)
        return;
    if(!DoesLightRespondToday(time, lightevent->dayOfWeek))
        return;
    if(lightevent->minuteOfDay != time->minuteOfDay)
        return;
    
    operateLight(lightevent);
}

void LightScheduler_Wakeup(void)
{
    Time time;
    TimeService_GetTime(&time);

    for (int i = 0; i < MAX_EVENTS; i++)
        processEventDueNow(&time, &scheduledEvents[i]);
}

static int scheduleEvent(int id, int event, int minutes, int day)
{
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        if (scheduledEvents[i].id == UNUSED)
        {
            scheduledEvents[i].id           = id;
            scheduledEvents[i].event        = event;
            scheduledEvents[i].minuteOfDay  = minutes;
            scheduledEvents[i].dayOfWeek    = day;
            return LS_OK;
        }
    }

    return LS_TOO_MANY_EVENTS;
}

int LightScheduler_ScheduleTurnOn(int id, int day, int minutes)
{
    return scheduleEvent(id, TURN_ON, minutes, day);
}

int LightScheduler_ScheduleTurnOff(int id, int day, int minutes)
{
    return scheduleEvent(id, TURN_OFF, minutes, day);
}