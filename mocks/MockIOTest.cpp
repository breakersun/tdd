
extern "C"
{
#include "MockIO.h"
}

#include "CppUTest/TestHarness.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"

static void MockIOSetup()
{
    MockIO_Create(2);
}

static void MockIODTeardown()
{
    MockIO_Destroy();
}

TEST_GROUP(MockIO)
{
    TestTestingFixture * fixture;
    int expectedErrors;

    void setup()
    {
        fixture = new TestTestingFixture();
        fixture->setSetup(MockIOSetup);
        fixture->setTeardown(MockIODTeardown);
        expectedErrors = 1;
    }
    void teardown()
    {
        delete fixture;
    }
    void testFailureWith(void (*method)())
    {
        fixture->setTestFunction(method);
        fixture->runAllTests();
        LONGS_EQUAL(expectedErrors, fixture->getFailureCount());
    }
};

static void CanCatchExpectations()
{
    // MockIO_Expect_ReadThenReturn(1, 2);
    MockIO_Expect_Write(1, 2);
    IO_Write(1, 1);
}

TEST(MockIO, CanCatchExpectations)
{
    expectedErrors = 0;
    testFailureWith(CanCatchExpectations);
    fixture->assertPrintContains("OK");
}