

extern "C"
{
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
#include "FakeRandomMinute.h"
}
#include "CppUTest/TestHarness.h"

TEST_GROUP(LightSchedulerRandomizeTest)
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

TEST(LightSchedulerRandomizeTest, TurnsOnEarly)
{
}