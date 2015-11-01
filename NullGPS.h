#ifndef __NULLGPS_H__
#define __NULLGPS_H__


#include "GPS.h"

class NullGPS : public GPS {

private:
  float lat;
  float lng;
  uint8_t counter;

public:
  NullGPS();

  void begin();
  void latlng(float* lat, float* lng);
  float speed();
  float altitude();
  float hdop();
  bool fix();
  float course();
  void datetime(int* year, byte* month, byte* day, byte* hour, byte* minute, byte* second);
  bool read();
  void dump();
  void query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);

};

#endif