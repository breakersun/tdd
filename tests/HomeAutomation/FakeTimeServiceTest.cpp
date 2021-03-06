#include "CppUTest/TestHarness.h"

extern "C"
{
#include "FakeTimeService.h"
#include "LightScheduler.h"
}

TEST_GROUP(FakeTimeService)
{
    void setup()
    {
        TimeService_Create();
    }

    void teardown()
    {
        TimeService_Destroy();
    }
};

TEST(FakeTimeService, Create)
{
    Time time;

    TimeService_GetTime(&time);
    LONGS_EQUAL(TIME_UNKNOWN, time.minuteOfDay);
    LONGS_EQUAL(TIME_UNKNOWN, time.dayOfWeek);
}

TEST(FakeTimeService, Set)
{
    FakeTimeService_SetMinute(1800);
    FakeTimeService_SetDay(SATURDAY);

    Time time;
    TimeService_GetTime(&time);
    LONGS_EQUAL(1800, time.minuteOfDay);
    LONGS_EQUAL(SATURDAY, time.dayOfWeek);
}