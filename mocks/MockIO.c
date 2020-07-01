
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CppUTest/TestHarness_c.h"
#include <MockIO.h>

enum {FLASH_READ, FLASH_WRITE, NoExpectedValue = -1};

typedef struct Expectation
{
    int         kind;
    ioAddress   addr;
    ioData      value;
}Expectation;

static Expectation * expectations = 0;
static int setExpectationCount;
static int getExpectationCount;
static int maxExpectationCount;

static Expectation expected, actual;

static int failureAlreadyReported;

static char * report_expect_write_was_read =
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t        But was IO_Read(0x%x)";
static char * report_read_wrong_address =
    "Expected IO_Read(0x%x) returns 0x%x;\n"
    "\t        But was IO_Read(0x%x)";
static char * report_expect_read_was_write =
    "Expected IO_Read(0x%x) would return 0x%x)\n"
    "\t        But was IO_Write(0x%x, 0x%x)";
static char * report_write_does_not_match =
    "Expected IO_Write(0x%x, 0x%x)\n"
    "\t        But was IO_Write(0x%x, 0x%x)";
static char * report_too_many_write_expectations =
    "MockIO_Expect_IO_Write: Too many expectations";
static char * report_too_many_read_expectations =
    "MockIO_Expect_IO_Read: Too many expectations";
static char * report_MockIO_not_initialized =
    "MockIO not initialized, call MockIO_Create()";
static char * report_write_but_out_of_expectations =
    "IO_Write(0x%x, 0x%x)";
static char * report_read_but_out_of_expectations =
    "IO_Read(0x%x)";
static const char * report_no_more_expectations =
    "R/W %d: No more expectations but was ";
static const char * report_expectation_number =
    "R/W %d: ";

void MockIO_Create(int maxExpectations)
{
    expectations = calloc(maxExpectations, sizeof(Expectation));
    setExpectationCount = 0;
    getExpectationCount = 0;
    maxExpectationCount = maxExpectations;
    failureAlreadyReported = 0;
}

void MockIO_Destroy(void)
{
    if (expectations)
        free(expectations);
    expectations = 0;
}

static void fail(char * message)
{
    failureAlreadyReported = 1;
    FAIL_TEXT_C(message);
}

static void failWhenNotInitialized(void)
{
    if (expectations == 0)
        fail(report_MockIO_not_initialized);
}

static void failWhenNoRoomForExpectations(char * msg)
{
    failWhenNotInitialized();
    if (setExpectationCount >= maxExpectationCount)
        fail(msg);
}

static void recordExpectation(int kind, ioAddress addr, ioData data)
{
    expectations[setExpectationCount].kind = kind;
    expectations[setExpectationCount].addr = addr;
    expectations[setExpectationCount].value = data;

    setExpectationCount++;
}

void MockIO_Expect_Write(ioAddress addr, ioData data)
{
    failWhenNoRoomForExpectations(report_too_many_write_expectations);
    recordExpectation(FLASH_WRITE, addr, data);
}

void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data)
{
    failWhenNoRoomForExpectations(report_too_many_read_expectations);
    recordExpectation(FLASH_READ, addr, data);
}

void static setExpectedAndActual(ioAddress addr, ioData data)
{
    expected.addr = expectations[getExpectationCount].addr;
    expected.value = expectations[getExpectationCount].value;

    actual.addr = addr;
    actual.value = data;
}

static void failWhenNoUnusedExpectations(char * format)
{
    char message[100];
    int size = sizeof message - 1;

    if (getExpectationCount >= setExpectationCount)
    {
        int offset = snprintf(message, size,
                report_no_more_expectations, getExpectationCount + 1);
        snprintf(message + offset, size - offset,
                format, actual.addr, actual.value);
        fail(message);
    }
}

static void failExpectation(char * expectationFailMessage)
{
    char msg[100];

    int offset = snprintf(msg, 
        sizeof(msg) - 1, 
        report_expectation_number, 
        getExpectationCount + 1);

    snprintf(msg + offset, 
        sizeof(msg) - 1 - offset, 
        expectationFailMessage, 
        expected.addr, 
        expected.value, 
        actual.addr, 
        actual.value);

    fail(msg);
}

static void failWhen(int condition, char * expectationFailMessage)
{
    if (condition)
        failExpectation(expectationFailMessage);
}

static int expectedKindIsNot(int kind)
{
    return kind != expectations[getExpectationCount].kind;
}

static int expectedAddressIsNot(ioAddress addr)
{
    return expected.addr != addr;
}

static int expectedDataIsNot(ioData data)
{
    return expected.value != data;
}


void IO_Write(ioAddress addr, ioData data)
{
    setExpectedAndActual(addr, data);
    failWhenNotInitialized();
    failWhenNoUnusedExpectations(report_write_but_out_of_expectations);
    failWhen(expectedKindIsNot(FLASH_WRITE), report_expect_read_was_write);
    failWhen(expectedAddressIsNot(addr), report_write_does_not_match);
    failWhen(expectedDataIsNot(data), report_write_does_not_match);
    getExpectationCount++;
}

ioData IO_Read(ioAddress addr)
{
    setExpectedAndActual(addr, NoExpectedValue);
    failWhenNotInitialized();
    failWhenNoUnusedExpectations(report_read_but_out_of_expectations);
    failWhen(expectedKindIsNot(FLASH_READ), report_expect_write_was_read);
    failWhen(expectedAddressIsNot(addr), report_read_wrong_address);

    return expectations[getExpectationCount++].value;
}

static void failWhenNotAllExpectationsUsed(void)
{
    char format[] = "Expected %d reads/writes but got %d";
    char msg[sizeof(format) + 5 + 5];

    if (getExpectationCount == setExpectationCount)
        return;

    snprintf(msg, sizeof(msg), format, setExpectationCount, getExpectationCount);
    fail(msg);
}

void MockIO_Verify_Complete(void)
{
    if (failureAlreadyReported)
        return;
    failWhenNotAllExpectationsUsed();
}