
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

enum {BUFFER_GUARD = -999};

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