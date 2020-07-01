
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

static void TooManyWriteExpectations()
{
    MockIO_Expect_Write(0, 1);
    MockIO_Expect_Write(0, 2);
    MockIO_Expect_Write(0, 3);
}
TEST(MockIO, TooManyWriteExpectations)
{
    testFailureWith(TooManyWriteExpectations);
    fixture->assertPrintContains("MockIO_Expect_IO_Write: Too many expectations");
}

static void TooManyReadExpectations()
{
    MockIO_Expect_ReadThenReturn(0, 1);
    MockIO_Expect_ReadThenReturn(0, 2);
    MockIO_Expect_ReadThenReturn(0, 3);
}
TEST(MockIO, TooManyReadExpectations)
{
    testFailureWith(TooManyReadExpectations);
    fixture->assertPrintContains("MockIO_Expect_IO_Read: Too many expectations");
}

static void NotInitReadTest()
{
    MockIO_Destroy();
    MockIO_Expect_ReadThenReturn(0, 0);
}
TEST(MockIO, NotInitReadTest)
{
    testFailureWith(NotInitReadTest);
    fixture->assertPrintContains("MockIO not initialized");
}

static void NotInitWriteTest()
{
    MockIO_Destroy();
    MockIO_Expect_Write(0, 0);
}
TEST(MockIO, NotInitWriteTest)
{
    testFailureWith(NotInitWriteTest);
    fixture->assertPrintContains("MockIO not initialized");
}

static void MismatchedWriteAddress()
{
    MockIO_Expect_Write(0, 0);
    IO_Write(1, 0);
}
TEST(MockIO, MismatchedWriteAddress)
{
    testFailureWith(MismatchedWriteAddress);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x0)");
    fixture->assertPrintContains("But was IO_Write(0x1, 0x0)");
}

static void MismatchWriteData()
{
    MockIO_Expect_Write(0, 1);
    IO_Write(0, 0);
}
TEST(MockIO, MismatchWriteData)
{
    testFailureWith(MismatchWriteData);
    fixture->assertPrintContains("Expected IO_Write(0x0, 0x1)");
    fixture->assertPrintContains("But was IO_Write(0x0, 0x0)");
}

static void MismatchReadAddress()
{
    MockIO_Expect_ReadThenReturn(0x1000, 0xaaaa);
    IO_Read(0x10000);
}
TEST(MockIO, MismatchReadAddress)
{
    testFailureWith(MismatchReadAddress);
    fixture->assertPrintContains("Expected IO_Read(0x1000) returns 0xaaaa");
    fixture->assertPrintContains("But was IO_Read(0x10000)");
}

static void TooManyReads()
{
    IO_Read(0x10000);
}
TEST(MockIO, TooManyReads)
{
    testFailureWith(TooManyReads);
    fixture->assertPrintContains("IO_Read(0x10000)");
}

static void TooManyWrites()
{
    IO_Write(0x10000, 0);
}
TEST(MockIO, TooManyWrites)
{
    testFailureWith(TooManyWrites);
    fixture->assertPrintContains("IO_Write(0x10000, 0x0)");
}

static void NotAllExpectationsUsed()
{
    MockIO_Expect_ReadThenReturn(0, 0);
    MockIO_Expect_Write(0, 0);
    MockIO_Verify_Complete();
}
TEST(MockIO, NotAllExpectationsUsed)
{
    testFailureWith(NotAllExpectationsUsed);
    fixture->assertPrintContains("Expected 2 reads/writes but got 0");
}