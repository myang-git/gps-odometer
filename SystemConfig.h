#ifndef __SYSTEMCONFIG_H__

#define __SYSTEMCONFIG_H__

#include <Adafruit_FRAM_I2C.h>

#include "StaticMemory.h"

static const uint8_t kNormalMode = 1;
static const uint8_t kTestMode = 2;
static const uint8_t kSystemConfigAddress = 64;

struct SystemConfig {
 uint8_t mode;
};

void readSystemConfig(Adafruit_FRAM_I2C* staticMemory, SystemConfig& config);
void writeSystemConfig(Adafruit_FRAM_I2C* staticMemory, const SystemConfig& config);

#endif