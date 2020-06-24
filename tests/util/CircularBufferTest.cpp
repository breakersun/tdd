
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "CircularBuffer.h"
#include "FormatOutputSpy.h"
}

TEST_GROUP(CircularBuffer)
{
    CircularBuffer buffer;

    void setup()
    {
        buffer = CircularBuffer_Create(10);
    }

    void teardown()
    {
        CircularBuffer_Destroy(buffer);
    }
};

TEST(CircularBuffer, Create)
{
}