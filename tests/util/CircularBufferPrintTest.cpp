
#include "CppUTest/TestHarness.h"

extern "C"
{
#include "CircularBuffer.h"
#include "FormatOutputSpy.h"
}

TEST_GROUP(CircularBufferPrint)
{
    CircularBuffer buffer;

    const char * expectedOutput;
    const char * actualOutput;

    void setup()
    {
        UT_PTR_SET(FormatOutput, FormatOutputSpy);
        FormatOutputSpy_Create(100);
        actualOutput = FormatOutputSpy_GetOutput();
        buffer = CircularBuffer_Create(10);
    }

    void teardown()
    {
        CircularBuffer_Destroy(buffer);
        FormatOutputSpy_Destroy();
    }

    void putManyInTheBuffer(int seed, int howMany)
    {
        for (int i = 0; i < howMany; i++)
            CircularBuffer_Put(buffer, i+seed);
    }
};

TEST(CircularBufferPrint, PrintEmpty)
{
    expectedOutput = "Circular buffer content:\n<>\n";
    CircularBuffer_Print(buffer);
    STRCMP_EQUAL(expectedOutput, actualOutput);
}