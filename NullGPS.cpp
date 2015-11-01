#include "NullGPS.h"


NullGPS::NullGPS() {
  this->lat = 40.729423;
  this->lng = -74.032063;
  this->counter = 0;
}

void NullGPS::begin() {
  Serial.println(F("nullgps ready"));
}

void NullGPS::latlng(float* lat, float* lng) {
  *lat = this->lat;
  *lng = this->lng;
  if ((this->counter % 2)==0) {
    this->lat = this->lat + 0.001;
    this->lng = this->lng + 0.001;
  }
  this->counter = (this->counter + 1) % 255;
}

float NullGPS::speed() {
  return 1.2215;
}

float NullGPS::altitude() {
  return 101.3;
}

float NullGPS::hdop() {
  return 7;
}

bool NullGPS::fix() {
  return true;
}

float NullGPS::course() {
  return 12.5;
}

void NullGPS::datetime(int* year, byte* month, byte* day, byte* hour, byte* minute, byte* second) {
  *year = 2015;
  *month = 8;
  *day = 22;
  *hour = 23;
  *minute = 2;
  *second = 0;
}

bool NullGPS::read() {
  return true;
}

void NullGPS::dump() {
}

void NullGPS::query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  *lat = this->lat;
  *lng = this->lng;
  *speed = this->speed();
  *altitude = this->altitude();
  *hdop = this->hdop();
  this->datetime(year, month, day, hour, minute, second);
}