#include "LightControllerSpy.h"
#include "memory.h"

static int lastId;
static int lastState;
static int lights[MAX_LIGHTS];

void LightController_Create(void)
{
    lastId      = LIGHT_ID_UNKNOWN;
    lastState   = LIGHT_STATE_UNKNOWN;
    memset(lights, LIGHT_STATE_UNKNOWN, sizeof(lights));
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

static int isIdInRange(int id)
{
    return (id >= 0 && id <= MAX_LIGHTS - 1);
}

void LightController_On(int Id)
{
    lastId      = Id;
    lastState   = LIGHT_ON;

    if (isIdInRange(Id))
        lights[Id] = LIGHT_ON;
}

void LightController_Off(int Id)
{
    lastId      = Id;
    lastState   = LIGHT_OFF;

    if (isIdInRange(Id))
        lights[Id] = LIGHT_OFF;
}

int LightControllerSpy_GetLightState(int id)
{
    if (!isIdInRange(id))
        return LIGHT_STATE_UNKNOWN;

    return lights[id];
}