
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
    CHECK_TRUE(CircularBuffer_VerifyIntegrity(buffer));
}

TEST(CircularBuffer, EmptyAfterCreatiion)
{
    CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

TEST(CircularBuffer, NotFullAfterCreation)
{
    CHECK_FALSE(CircularBuffer_IsFull(buffer));
}

TEST(CircularBuffer, NotEmptyAfterInput)
{
    CircularBuffer_Put(buffer, 10046);
    CHECK_FALSE(CircularBuffer_IsEmpty(buffer));
}

TEST(CircularBuffer, NotEmptyThenEmpty)
{
    CircularBuffer_Put(buffer, 10046);
    CHECK_FALSE(CircularBuffer_IsEmpty(buffer));
    CircularBuffer_Get(buffer);
    CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

TEST(CircularBuffer, PutGetOneValue)
{
    CircularBuffer_Put(buffer, 10046);
    LONGS_EQUAL(10046, CircularBuffer_Get(buffer));
}

TEST(CircularBuffer, PutGetAFew)
{
    CircularBuffer_Put(buffer, 10046);
    CircularBuffer_Put(buffer, 10047);
    CircularBuffer_Put(buffer, 10048);
    LONGS_EQUAL(10046, CircularBuffer_Get(buffer));
    LONGS_EQUAL(10047, CircularBuffer_Get(buffer));
    LONGS_EQUAL(10048, CircularBuffer_Get(buffer));
}

TEST(CircularBuffer, Capacity)
{
    CircularBuffer b = CircularBuffer_Create(2);
    LONGS_EQUAL(2, CircularBuffer_Capacity(b));
    CircularBuffer_Destroy(b);
}

TEST(CircularBuffer, IsFull)
{
    for (int i = 0; i < CircularBuffer_Capacity(buffer); i++)
        CircularBuffer_Put(buffer, i + 100);

    CHECK_TRUE(CircularBuffer_IsFull(buffer));
}

TEST(CircularBuffer, EmptyToFullToEmpty)
{
    for (int i = 0; i < CircularBuffer_Capacity(buffer); i++)
        CircularBuffer_Put(buffer, i + 100);

    CHECK_TRUE(CircularBuffer_IsFull(buffer));

    for (int j = 0; j < CircularBuffer_Capacity(buffer); j++)
        LONGS_EQUAL(j + 100, CircularBuffer_Get(buffer));

    CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
}

TEST(CircularBuffer, WrapAround)
{
    int capacity = CircularBuffer_Capacity(buffer);

    for (int i = 0; i < capacity; i++)
        CircularBuffer_Put(buffer, i + 100);
    
    CHECK_TRUE(CircularBuffer_IsFull(buffer));
    LONGS_EQUAL(100, CircularBuffer_Get(buffer));
    CHECK_FALSE(CircularBuffer_IsFull(buffer));
    CircularBuffer_Put(buffer, 1000);
    CHECK_TRUE(CircularBuffer_IsFull(buffer));

    for (int j = 1; j < capacity; j++)
        LONGS_EQUAL(j + 100, CircularBuffer_Get(buffer));

    LONGS_EQUAL(1000, CircularBuffer_Get(buffer));
    CHECK_TRUE(CircularBuffer_IsEmpty(buffer));
    CHECK_TRUE(CircularBuffer_VerifyIntegrity(buffer));
}

TEST(CircularBuffer, PutToFullThrows)
{

}

TEST(CircularBuffer, PutToFullNotDamageContents)
{

}

TEST(CircularBuffer, GetFromEmptyReturns0)
{

}