
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
    self->count++;
    self->values[self->index++] = value;

    return 1;
}

int CircularBuffer_Get(CircularBuffer self)
{
    self->count--;
    return (self->values[self->outdex++]);
}

int CircularBuffer_Capacity(CircularBuffer self)
{
    return self->capacity;
}