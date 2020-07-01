#ifndef __MOCKIO_H__
#define __MOCKIO_H__

#include "IO.h"

void MockIO_Create(int maxExpectations);
void MockIO_Destroy(void);
void MockIO_Expect_Write(ioAddress addr, ioData data);
void MockIO_Expect_ReadThenReturn(ioAddress addr, ioData data);
void MockIO_Verify_Complete(void);

#endif