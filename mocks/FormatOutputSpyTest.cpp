

#include "CppUTest/TestHarness.h"

extern "C"
{
#include "FormatOutputSpy.h"
}

TEST_GROUP(FormatOutputSpy)
{
    void setup()
    {
        UT_PTR_SET(FormatOutput, FormatOutputSpy);
    }

    void teardown()
    {
        FormatOutputSpy_Destroy();
    }
};


TEST(FormatOutputSpy, HelloWorld)
{
    FormatOutputSpy_Create(20);
    FormatOutput("Hello, World\n");
    STRCMP_EQUAL("Hello, World\n", FormatOutputSpy_GetOutput());
}
