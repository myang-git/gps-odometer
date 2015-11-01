#ifndef __TINYGPSPLUSADAPTER_H__
#define __TINYGPSPLUSADAPTER_H__

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "GPS.h"

class TinyGPSPlusAdapter : public GPS {

private:
  SoftwareSerial* swserial;
  TinyGPSPlus gps;

public:
  TinyGPSPlusAdapter(SoftwareSerial* swserial);

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