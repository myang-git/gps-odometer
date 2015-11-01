#ifndef __GPSLogger_h__
#define __GPSLogger_h__

#include <stdint.h>
#include <Adafruit_FRAM_I2C.h>

#include "StaticMemory.h"
#include "GPS.h"
#include "LoggerData.h"

#define kLoggerDataStatusAddress  0
#define kLoggerDataAddress        1

const uint8_t kLoggerDataAvailable = 0x88;


#define kHomeLat                  40.729423
#define kHomeLng                  -74.032063

#define kMinLogDistance           5
#define kMinLogSpeed              1

class GPSLogger {
private:

  LoggerData loggerData;
  volatile bool lastLocationAvailable;
  volatile float lastLat;
  volatile float lastLng;
  GPS* gps;
  Adafruit_FRAM_I2C* staticMemory;
  //static const uint8_t kLoggerDataAvailable;

  void writeLoggerData();
  void readLoggerData();
  void locateHome(float lat, float lng);
  uint32_t dist(float lat1, float lng1, float lat2, float lng2);

public:
  GPSLogger();
  GPSLogger(GPS* gps, Adafruit_FRAM_I2C* staticMemory);
  ~GPSLogger();
  void begin();
  char pollGPS();
  bool hasNewUpdate();
  void log();
  void resetLastLocation();
  const LoggerData& getLoggerData();
  unsigned long getDistanceTravelled();

  void setDistanceTravelled(long dist);
};



#endif