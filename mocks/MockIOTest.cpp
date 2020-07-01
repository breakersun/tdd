
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
    IO_Write(1, 2);
}

TEST(MockIO, CanCatchExpectations)
{
    expectedErrors = 0;
    testFailureWith(CanCatchExpectations);
    fixture->assertPrintContains("OK");
}

static void WriteWhenReadExpectedFails()
{
    MockIO_Expect_ReadThenReturn(0, 1);
    IO_Write(0, 0);
}
TEST(MockIO, WriteWhenReadExpectedFails)
{
    testFailureWith(WriteWhenReadExpectedFails);
    fixture->assertPrintContains("Expected IO_Read(0x0) would return 0x1");
    fixture->assertPrintContains("But was IO_Write(0x0, 0x0)");
}

static void ReadWhenWriteExpectedFails()
{
    MockIO_Expect_Write(0, 1);
    IO_Read(0);
}
TEST(MockIO, ReadWhenWriteExpectedFails)
{
    testFailureWith(ReadWhenWriteExpectedFails);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x1)");
    fixture->assertPrintContains("But was IO_Read(0x0)");
}