#include "LightScheduler.h"
#include "LightController.h"

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
    int day;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{

}

static void operateLight(ScheduledLightEvent *lightevent)
{
    if(lightevent->event == TURN_ON)
        LightController_On(lightevent->id);
    else if(lightevent->event == TURN_OFF)
        LightController_Off(lightevent->id);
}

static void processEventDueNow(Time *time, ScheduledLightEvent *lightevent)
{
    if(lightevent->id == UNUSED)
        return;

    if(lightevent->minuteOfDay != time->minuteOfDay)
        return;

    if(lightevent->day != EVERYDAY)
        return;

    operateLight(lightevent);
}

void LightScheduler_Wakeup(void)
{
    Time time;
    TimeService_GetTime(&time);

    processEventDueNow(&time, &scheduledEvent);
}

static void scheduleEvent(int id, int event, int minutes, int day)
{
    scheduledEvent.id           = id;
    scheduledEvent.event        = event;
    scheduledEvent.minuteOfDay  = minutes;
    scheduledEvent.day          = day;
}

void LightScheduler_ScheduleTurnOn(int id, int day, int minutes)
{
    scheduleEvent(id, TURN_ON, minutes, day);
}

void LightScheduler_ScheduleTurnOff(int id, int day, int minutes)
{
    scheduleEvent(id, TURN_OFF, minutes, day);
}