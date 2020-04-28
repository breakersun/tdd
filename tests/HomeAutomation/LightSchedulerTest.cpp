
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
}

TEST_GROUP(LightScheduler)
{
    void setup()
    {
        LightController_Create();
        LightScheduler_Create();
    }

    void teardown()
    {
        LightController_Destroy();
        LightScheduler_Destroy();
    }

    //helpers
    void setTimeTo(int day, int minute)
    {
        FakeTimeService_SetDay(day);
        FakeTimeService_SetMinute(minute);
    }

    void checkLedState(int id, int level)
    {
        if (id == LIGHT_ID_UNKNOWN)
        {
            LONGS_EQUAL(id, LightControllerSpy_GetLastId());
            LONGS_EQUAL(level, LightControllerSpy_GetLastState());
        }
        else
            LONGS_EQUAL(level, LightControllerSpy_GetLightState(id));
    }
};

TEST(LightScheduler, NoChangeToLightsDuringInit)
{
    checkLedState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, NoScheduleNothingHappens)
{
    setTimeTo(MONDAY, 1000);
    LightScheduler_Wakeup();
    checkLedState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleOnEverydayNotTimeYet)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1000);
    LightScheduler_Wakeup();
    checkLedState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleOnEverydayItsTime)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(3, LIGHT_ON);
}

TEST(LightScheduler, ScheduleOffEverydayItsTime)
{
    LightScheduler_ScheduleTurnOff(18, EVERYDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(18, LIGHT_OFF);
}

TEST(LightScheduler, ScheduleTuesdayButItsMonday)
{
    LightScheduler_ScheduleTurnOff(3, TUESDAY, 1200);
    setTimeTo(MONDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleTuesdayItsTuesday)
{
    LightScheduler_ScheduleTurnOff(3, TUESDAY, 1200);
    setTimeTo(TUESDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(3, LIGHT_OFF);    
}

TEST(LightScheduler, ScheduleWeekendButItsFriday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(FRIDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

TEST(LightScheduler, ScheduleWeekendAndItsSunday)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(3, LIGHT_ON);   
}

TEST(LightScheduler, ScheduleTwoEventsAtTheSameTime)
{
    LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);
    LightScheduler_ScheduleTurnOn(12, WEEKEND, 1200);
    setTimeTo(SUNDAY, 1200);
    LightScheduler_Wakeup();
    checkLedState(3, LIGHT_ON);
    checkLedState(12, LIGHT_ON);
}

TEST(LightScheduler, RejectTooManyEvents)
{
    for (int i = 0; i < 128; i++)
        LONGS_EQUAL(LS_OK, LightScheduler_ScheduleTurnOn(6, WEEKEND, i + 600));

    LONGS_EQUAL(LS_TOO_MANY_EVENTS, LightScheduler_ScheduleTurnOn(6, WEEKEND, 600));
}

TEST_GROUP(LightSchedulerInitAndCleanup)
{};

TEST(LightSchedulerInitAndCleanup, CreateStartsOneMinuteAlarm)
{
    LightScheduler_Create();
    POINTERS_EQUAL((void *)LightScheduler_Wakeup, (void *)FakeTimeService_GetAlarmCallback());
    LONGS_EQUAL(60, FakeTimeService_GetAlarmPeriod());
}

TEST(LightSchedulerInitAndCleanup, DestroyCancelsOneMinuteAlarm)
{
    LightScheduler_Create();
    LightScheduler_Destroy();
    POINTERS_EQUAL(NULL, (void *)FakeTimeService_GetAlarmCallback());
    LONGS_EQUAL(0, FakeTimeService_GetAlarmPeriod());
}