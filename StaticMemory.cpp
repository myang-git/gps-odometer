#include "StaticMemory.h"

void staticMemoryWrite(Adafruit_FRAM_I2C* staticMemory, int& address, const void* value, size_t size) {
  int endAddress = address + size;

  byte* ptr = (byte*)value;
  for (int i=0;address<endAddress;address++, i++) {
    staticMemory->write8(address, ptr[i]);
  }
}

void staticMemoryRead(Adafruit_FRAM_I2C* staticMemory, int& address, void* value, size_t size) {
  int endAddress = address + size;
  byte* ptr = (byte*)value;
  for (int i=0;address<endAddress;address++, i++) {
    byte b = staticMemory->read8(address);
    ptr[i] = b;
  }
}
