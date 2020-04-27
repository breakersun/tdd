#include "CppUTest/TestHarness.h"

extern "C"
{
#include "FakeTimeService.h"
// #include "LightScheduler.h"
// #include "memory.h"
}

TEST_GROUP(FakeTimeService)
{
    void setup()
    {
        TimeService_Creat();
    }

    void teardown()
    {
        TimeService_Destroy();
    }
};

// TEST(FakeTimeService, Create)
// {
//     Time time;

//     LONGS_EQUAL()
// }