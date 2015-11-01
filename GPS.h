#ifndef __GPS_H__
#define __GPS_H__

#include <Arduino.h>

#define kGPSBaudRate  9600
#define PMTK_CMD_HOT_START "$PMTK101*32"
#define PMTK_CMD_WARM_START "PMTK102*31"
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E"
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

class GPS {

public:
  virtual void begin();
  virtual void latlng(float* lat, float* lng);
  virtual float speed();
  virtual float altitude();
  virtual float hdop();
  virtual bool fix();
  virtual float course();
  virtual void datetime(int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);
  virtual bool read();
  virtual void dump();
  virtual void query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);
};


#endif