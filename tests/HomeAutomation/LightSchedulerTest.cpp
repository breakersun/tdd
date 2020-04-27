
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
        LONGS_EQUAL(id, LightControllerSpy_GetLastId());
        LONGS_EQUAL(level, LightControllerSpy_GetLastState());
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