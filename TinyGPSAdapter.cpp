#include "TinyGPSAdapter.h"


TinyGPSAdapter::TinyGPSAdapter(SoftwareSerial* swserial) {
  this->swserial = swserial;
}

void TinyGPSAdapter::begin() {
  this->swserial->begin(kGPSBaudRate);
  this->swserial->println(PMTK_CMD_HOT_START);
  this->swserial->println(PMTK_ENABLE_WAAS);
  this->swserial->println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  this->swserial->println(PMTK_SET_NMEA_UPDATE_1HZ);
  this->swserial->println(PMTK_API_SET_FIX_CTL_1HZ);
  delay(1000);
  Serial.println(F("tinygps ready"));
}

void TinyGPSAdapter::latlng(float* lat, float* lng) {
  this->gps.f_get_position(lat, lng);
}

float TinyGPSAdapter::speed() {
  return (this->fix() ? this->gps.f_speed_kmph() : 0);
}

float TinyGPSAdapter::altitude() {
  return (this->fix() ? this->gps.f_altitude() : 0);
}

float TinyGPSAdapter::hdop() {
  return (this->fix() ? this->gps.hdop() / 100.0 : 9999);
}

bool TinyGPSAdapter::fix() {
  float lat, lng;
  unsigned long fix_age;
  this->gps.f_get_position(&lat, &lng, &fix_age);  
  return fix_age!=TinyGPS::GPS_INVALID_AGE && lat!=TinyGPS::GPS_INVALID_ANGLE && lng!=TinyGPS::GPS_INVALID_ANGLE;
}

float TinyGPSAdapter::course() {
  return (this->fix() ? this->gps.course() : 0);
}

void TinyGPSAdapter::datetime(int* year, byte* month, byte* day, byte* hour, byte* minute, byte* second) {
  byte hundredths;
  unsigned long fix_age;
  this->gps.crack_datetime(year, month, day, hour, minute, second, &hundredths, &fix_age);  
}

bool TinyGPSAdapter::read() {
  if (!this->swserial->available()) {
    return 0;
  }
  byte data = this->swserial->read();
  return this->gps.encode(data);
}

void TinyGPSAdapter::dump() {
  int year;
  byte month, day, hour, minute, second;
  byte hundredths;
  long lat, lng;
  unsigned long fix_age;
  this->gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &fix_age);  
  this->gps.get_position(&lat, &lng, &fix_age);
  Serial.print("\nTime: ");
  Serial.print(hour, DEC); Serial.print(':');
  Serial.print(minute, DEC); Serial.print(':');
  Serial.print(second, DEC); Serial.print('.');
  Serial.println(hundredths);
  Serial.print("Date: ");
  Serial.print(day, DEC); Serial.print('/');
  Serial.print(month, DEC); Serial.print("/20");
  Serial.println(year, DEC);
  Serial.print("Fix Age: "); Serial.println(fix_age);
  if (fix_age != TinyGPS::GPS_INVALID_AGE) {
    Serial.print("Location: ");
    Serial.print(lat);
    Serial.print(", "); 
    Serial.println(lng);
    Serial.print("Speed: "); Serial.println(this->gps.f_speed_kmph());
    Serial.print("Angle: "); Serial.println(this->gps.course());
    Serial.print("Altitude: "); Serial.println(this->gps.f_altitude());
  }  
}

void TinyGPSAdapter::query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  unsigned long fix_age = 0;
  byte hundredths;
  this->gps.f_get_position(lat, lng, &fix_age);
  *speed = this->gps.f_speed_kmph();
  *altitude = this->gps.f_altitude();
  *hdop = this->gps.hdop();
  this->gps.crack_datetime(year, month, day, hour, minute, second, &hundredths, &fix_age);  
}