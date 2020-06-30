
#include "CircularBuffer.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>

typedef struct CircularBufferStruct
{
    int count;
    int index;
    int outdex;
    int capacity;
    int * values;
}CircularBufferSturct;

enum {BUFFER_GUARD = 0xDEADBEAF};

CircularBuffer CircularBuffer_Create(int capacity)
{
    CircularBuffer self     = calloc(1, sizeof(CircularBufferSturct));
    self->capacity          = capacity;
    self->values            = calloc(capacity + 1, sizeof(int));
    self->values[capacity]  = BUFFER_GUARD;

    return self;
}

void CircularBuffer_Destroy(CircularBuffer self)
{
    free(self->values);
    free(self);
}

int CircularBuffer_VerifyIntegrity(CircularBuffer self)
{
    return (self->values[self->capacity] == BUFFER_GUARD);
}

int CircularBuffer_IsEmpty(CircularBuffer self)
{
    return (self->count == 0);
}

int CircularBuffer_IsFull(CircularBuffer self)
{
    return (self->count == self->capacity);
}

int CircularBuffer_Put(CircularBuffer self, int value)
{
    if (self->count >= self->capacity)
        return 0;

    self->count++;
    self->values[self->index++] = value;

    if (self->index >= self->capacity)
        self->index = 0;

    return 1;
}

int CircularBuffer_Get(CircularBuffer self)
{
    int value;

    if (self->count <= 0)
        return -1;

    self->count--;
    value = self->values[self->outdex++];

    if (self->outdex >= self->capacity)
        self->outdex = 0;

    return value;
}

int CircularBuffer_Capacity(CircularBuffer self)
{
    return self->capacity;
}

void CircularBuffer_Print(CircularBuffer self)
{
    int currentValue = self->outdex;

    FormatOutput("Circular buffer content:\n<");

    for (int i = 0; i < self->count; i++)
    {
        if (i > 0)
            FormatOutput(", ");
        FormatOutput("%d", self->values[currentValue++]);

        if (currentValue >= self->capacity)
            currentValue = 0;
    }

    FormatOutput(">\n");
}