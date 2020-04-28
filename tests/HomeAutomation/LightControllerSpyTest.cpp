#include "CppUTest/TestHarness.h"

extern "C"
{
#include "LightControllerSpy.h"
}

TEST_GROUP(LightControllerSpy)
{
    void setup()
    {
      LightController_Create();
    }

    void teardown()
    {
       LightController_Destroy();
    }
};

TEST(LightControllerSpy, Creat)
{
  LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
  LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightControllerSpy, RememberTheLastLightIdControlled)
{
  LightController_On(8);
  LONGS_EQUAL(8, LightControllerSpy_GetLastId());
  LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLastState());
  LightController_Off(10);
  LONGS_EQUAL(10, LightControllerSpy_GetLastId());
  LONGS_EQUAL(LIGHT_OFF, LightControllerSpy_GetLastState());
}

TEST(LightControllerSpy, RememberAllLightsStates)
{
  LightController_On(0);
  LightController_Off(31);
  LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLightState(0));
  LONGS_EQUAL(LIGHT_OFF, LightControllerSpy_GetLightState(31));
}