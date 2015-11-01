#ifndef __ADAFRUITGPSADAPTER_H__
#define __ADAFRUITGPSADAPTER_H__

#include <Adafruit_GPS.h>
#include "GPS.h"

#define kKPHPerKnot   1.85200
#define kGPSBaudRate  9600

class AdafruitGPSAdapter : public GPS {

private:
  SoftwareSerial* swserial;
  Adafruit_GPS gps;

public:
  AdafruitGPSAdapter(SoftwareSerial* swserial);
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