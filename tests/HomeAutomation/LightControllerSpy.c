#include "LightControllerSpy.h"

static int lastId;
static int lastState;

void LightController_Create(void)
{
    lastId      = LIGHT_ID_UNKNOWN;
    lastState   = LIGHT_STATE_UNKNOWN;
}

void LightController_Destroy(void)
{
    return;
}

int LightControllerSpy_GetLastId(void)
{
    return lastId;
}

int LightControllerSpy_GetLastState(void)
{
    return lastState;
}

void LightController_On(int Id)
{
    lastId      = Id;
    lastState   = LIGHT_ON;
}

void LightController_Off(int Id)
{
    lastId      = Id;
    lastState   = LIGHT_OFF;
}

int LightControllerSpy_GetLightState(int id)
{
    return lastState;
}