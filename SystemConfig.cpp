#include "SystemConfig.h"

//void staticMemoryWrite(Adafruit_FRAM_I2C* staticMemory, int& address, const void* const value, size_t size);
//void staticMemoryRead(Adafruit_FRAM_I2C* staticMemory, int& address, void* value, size_t size);

void readSystemConfig(Adafruit_FRAM_I2C* staticMemory, SystemConfig& config) {
  int address = kSystemConfigAddress;
  staticMemoryRead(staticMemory, address, &config.mode, sizeof(config.mode));
}

void writeSystemConfig(Adafruit_FRAM_I2C* staticMemory, const SystemConfig& config) {
  int address = kSystemConfigAddress;
  staticMemoryWrite(staticMemory, address, &config.mode, sizeof(config.mode));
}
