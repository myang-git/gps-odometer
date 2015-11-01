#ifndef __LOGGERDATA_H__

#define __LOGGERDATA_H__

#define kKPHPerKnot               1.85200

struct LoggerData {
  uint32_t distanceTravelled;
  uint32_t distanceFromHome;
  uint32_t distanceTravelledScaled;
  uint32_t tripDistance;
  uint32_t tripDistanceScaled;
  uint32_t timeInMotion;
  uint32_t lastDistance;
  uint8_t homeLatDir;
  uint8_t homeLngDir;
  float lastLat;
  float lastLng;
  float maxSpeed;
  float maxAltitude;
  float minAltitude;
  bool logged;
  bool lastLocationAvailable;
  uint32_t logCounter;
};

#endif