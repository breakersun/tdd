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
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

void LightScheduler_Create(void)
{
    scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{

}

void LightScheduler_Wakeup(void)
{
    Time time;
    TimeService_GetTime(&time);

    if (scheduledEvent.id == UNUSED)
        return;
    
    if(time.minuteOfDay != scheduledEvent.minuteOfDay)
        return;

    if(scheduledEvent.event == TURN_ON)
        LightController_On(scheduledEvent.id);
    else if(scheduledEvent.event == TURN_OFF)
        LightController_Off(scheduledEvent.id);
}

static void scheduleEvent(int id, int event, int minutes)
{
    scheduledEvent.id           = id;
    scheduledEvent.event        = event;
    scheduledEvent.minuteOfDay  = minutes;
}

void LightScheduler_ScheduleTurnOn(int id, int day, int minutes)
{
    scheduleEvent(id, TURN_ON, minutes);
}

void LightScheduler_ScheduleTurnOff(int id, int day, int minutes)
{
    scheduleEvent(id, TURN_OFF, minutes);
}