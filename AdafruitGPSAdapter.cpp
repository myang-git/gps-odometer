#include "AdafruitGPSAdapter.h"

AdafruitGPSAdapter::AdafruitGPSAdapter(SoftwareSerial* swserial) : gps(swserial) {
  this->swserial = swserial;
}

void AdafruitGPSAdapter::begin() {
  this->gps.begin(kGPSBaudRate);
  this->swserial->println(PMTK_CMD_HOT_START);
  this->swserial->println(PMTK_ENABLE_WAAS);
  this->swserial->println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  this->swserial->println(PMTK_SET_NMEA_UPDATE_1HZ);
  this->swserial->println(PMTK_API_SET_FIX_CTL_1HZ);
  delay(1000);
  Serial.println(F("adafruit gps ready"));
}

void AdafruitGPSAdapter::latlng(float* lat, float* lng) {
  *lat = this->gps.latitude_fixed / 100000.0;
  *lng = this->gps.longitude_fixed / 100000.0;
}

float AdafruitGPSAdapter::speed() {
  return this->fix() ? this->gps.speed * kKPHPerKnot : 0;
}

float AdafruitGPSAdapter::altitude() {
  return this->fix() ? this->gps.altitude : 0;
}

float AdafruitGPSAdapter::hdop() {
  return this->fix() ? this->gps.HDOP : 9999;
}

bool AdafruitGPSAdapter::fix() {
  return this->gps.fix;
}

float AdafruitGPSAdapter::course() {
  return this->fix() ? this->gps.angle : 0;
}

void AdafruitGPSAdapter::datetime(int* year, byte* month, byte* day, byte* hour, byte* minute, byte* second) {
  *year = this->gps.year;
  *month = this->gps.month;
  *day = this->gps.day;
  *hour = this->gps.hour;
  *minute = this->gps.minute;
  *second = this->gps.seconds;
}

bool AdafruitGPSAdapter::read() {
  this->gps.read();
  if (this->gps.newNMEAreceived()) {
    return this->gps.parse(this->gps.lastNMEA());
  }
  else {
    return false;
  }
}

void AdafruitGPSAdapter::dump() {
    Serial.print("\nTime: ");
    Serial.print(this->gps.hour, DEC); Serial.print(':');
    Serial.print(this->gps.minute, DEC); Serial.print(':');
    Serial.print(this->gps.seconds, DEC); Serial.print('.');
    Serial.println(this->gps.milliseconds);
    Serial.print("Date: ");
    Serial.print(this->gps.day, DEC); Serial.print('/');
    Serial.print(this->gps.month, DEC); Serial.print("/20");
    Serial.println(this->gps.year, DEC);
    Serial.print("Fix: "); Serial.print((int)this->gps.fix);
    Serial.print(" quality: "); Serial.println((int)this->gps.fixquality); 
    if (this->gps.fix) {
      Serial.print("Location: ");
      Serial.print(this->gps.latitude, 4); Serial.print(this->gps.lat);
      Serial.print(", "); 
      Serial.print(this->gps.longitude, 4); Serial.println(this->gps.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(this->gps.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(this->gps.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(this->gps.speed);
      Serial.print("Angle: "); Serial.println(this->gps.angle);
      Serial.print("Altitude: "); Serial.println(this->gps.altitude);
      Serial.print("Satellites: "); Serial.println((int)this->gps.satellites);
    }
    
}

void AdafruitGPSAdapter::query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  *lat = this->gps.latitude;
  *lng = this->gps.longitude;
  *speed = this->gps.speed;
  *altitude = this->gps.altitude;
  *hdop = this->gps.HDOP;
  *year = this->gps.year;
  *month = this->gps.month;
  *day = this->gps.day;
  *hour = this->gps.hour;
  *minute = this->gps.minute;
  *second = this->gps.seconds;
}