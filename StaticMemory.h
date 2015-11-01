#ifndef __STATICMEMORY_H__

#define __STATICMEMORY_H__

#include <Adafruit_FRAM_I2C.h>

void staticMemoryWrite(Adafruit_FRAM_I2C* staticMemory, int& address, const void* const value, size_t size);
void staticMemoryRead(Adafruit_FRAM_I2C* staticMemory, int& address, void* value, size_t size);

#endif