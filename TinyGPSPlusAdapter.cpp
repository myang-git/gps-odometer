#include <TinyGPSPlusAdapter.h>

TinyGPSPlusAdapter::TinyGPSPlusAdapter(SoftwareSerial* swserial) {
  this->swserial = swserial;
}

void TinyGPSPlusAdapter::begin() {
  this->swserial->begin(kGPSBaudRate);

  this->swserial->println(PMTK_CMD_HOT_START);
  this->swserial->println(PMTK_ENABLE_WAAS);
  this->swserial->println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  this->swserial->println(PMTK_SET_NMEA_UPDATE_1HZ);
  this->swserial->println(PMTK_API_SET_FIX_CTL_1HZ);
  delay(1000);
  Serial.println(F("tinygps plus ready"));
}

void TinyGPSPlusAdapter::latlng(float* lat, float* lng) {
  *lat = this->gps.location.lat();
  *lng = this->gps.location.lng();
}

float TinyGPSPlusAdapter::speed() {
  return (this->gps.speed.isValid() ? this->gps.speed.kmph() : 0);
}

float TinyGPSPlusAdapter::altitude() {
  return (this->gps.altitude.isValid() ? this->gps.altitude.meters() : 0);
}

float TinyGPSPlusAdapter::hdop() {
  return (this->fix() ? this->gps.hdop.value() / 100.0 : 9999);
}

bool TinyGPSPlusAdapter::fix() {
  return this->gps.location.isValid();
}

float TinyGPSPlusAdapter::course() {
  return (this->gps.course.isValid() ? this->gps.course.deg() : 0);
}

void TinyGPSPlusAdapter::datetime(int* year, byte* month, byte* day, byte* hour, byte* minute, byte* second) {
  *year = this->gps.date.year();
  *month = this->gps.date.month();
  *day = this->gps.date.day();
  *hour = this->gps.time.hour();
  *minute = this->gps.time.minute();
  *second = this->gps.time.second();
}

bool TinyGPSPlusAdapter::read() {
  if (!this->swserial->available()) {
    return 0;
  }
  byte data = this->swserial->read();
  return this->gps.encode(data);
}

void TinyGPSPlusAdapter::dump() {
}

void TinyGPSPlusAdapter::query(float* lat, float* lng, float* speed, float* altitude, float* hdop, int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  *lat = this->gps.location.lat();
  *lng = this->gps.location.lng();
  *speed = this->gps.speed.kmph();
  *altitude = this->gps.altitude.meters();
  *hdop = this->gps.hdop.value();
  *year = this->gps.date.year();
  *month = this->gps.date.month();
  *day = this->gps.date.day();
  *hour = this->gps.time.hour();
  *minute = this->gps.time.minute();
  *second = this->gps.time.second();
}