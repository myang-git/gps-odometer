#include "GPSLogger.h"


//SoftwareSerial mySerial(3, 2);
//TinyGPS gGPS(&mySerial);

GPSLogger::GPSLogger() {
  //this->gps = &gGPS;
}

GPSLogger::GPSLogger(GPS* gps, Adafruit_FRAM_I2C* staticMemory) {
  this->gps = gps;
  this->staticMemory = staticMemory;
}

GPSLogger::~GPSLogger() {
}

void GPSLogger::begin() {
  readLoggerData();
  this->loggerData.lastLat = 0;
  this->loggerData.lastLng = 0;
  this->loggerData.logCounter = 0;
  this->lastLocationAvailable = false;
  Serial.println(F("GPS logger ready"));
}

uint32_t GPSLogger::getDistanceTravelled() {
  return this->loggerData.distanceTravelled;
}

void GPSLogger::locateHome(float lat, float lng) {
  if (kHomeLat>lat) {
    this->loggerData.homeLatDir = 'N';
  }
  else {
    this->loggerData.homeLatDir = 'S';
  }
  if (kHomeLng>lng) {
    this->loggerData.homeLngDir = 'E';
  }
  else {
    this->loggerData.homeLngDir = 'W';
  }
}

void GPSLogger::log() {
  float lat, lng;
  this->gps->latlng(&lat, &lng);
  if (this->lastLocationAvailable) {
    uint32_t distIncrement = this->dist(this->lastLat, this->lastLng, lat, lng);
    if (this->gps->speed()>=kMinLogSpeed || distIncrement>=kMinLogDistance) {
      this->loggerData.lastDistance = distIncrement;
      this->loggerData.distanceTravelled+=distIncrement;
      this->loggerData.tripDistance+=distIncrement;
      this->loggerData.distanceTravelledScaled = this->loggerData.distanceTravelled / 100;
      this->loggerData.tripDistanceScaled = this->loggerData.tripDistance / 100;
      this->lastLat = lat;
      this->lastLng = lng;
      this->loggerData.lastLat = lat;
      this->loggerData.lastLng = lng;
      this->loggerData.logged = true;
      this->writeLoggerData();
    }
  }
  else {
    this->lastLocationAvailable = true;
    this->lastLat = lat;
    this->lastLng = lng;
  }
  float currentSpeed = this->gps->speed();
  float currentAltitude = this->gps->altitude();
  if (currentSpeed > this->loggerData.maxSpeed) {
    this->loggerData.maxSpeed = currentSpeed;
  }
  if (currentAltitude > this->loggerData.maxAltitude) {
    this->loggerData.maxAltitude = currentAltitude;
  }
  if (currentAltitude < this->loggerData.minAltitude) {
    this->loggerData.minAltitude = currentAltitude;
  }
  this->loggerData.distanceFromHome = this->dist(kHomeLat, kHomeLng, lat, lng);
  this->locateHome(lat, lng);
  this->loggerData.logCounter++;
}

void GPSLogger::resetLastLocation() {
  this->lastLat = 0;
  this->lastLng = 0;
  this->lastLocationAvailable = false;
  this->loggerData.logged = false;
}

void GPSLogger::writeLoggerData() {
  int address = kLoggerDataStatusAddress;
  staticMemoryWrite(this->staticMemory, address, &kLoggerDataAvailable, sizeof(uint8_t));
  address = kLoggerDataAddress;
  staticMemoryWrite(this->staticMemory, address, &this->loggerData.distanceTravelled, sizeof(this->loggerData.distanceTravelled));
  staticMemoryWrite(this->staticMemory, address, &this->loggerData.timeInMotion, sizeof(this->loggerData.timeInMotion));
  staticMemoryWrite(this->staticMemory, address, &this->loggerData.maxSpeed, sizeof(this->loggerData.maxSpeed));
  staticMemoryWrite(this->staticMemory, address, &this->loggerData.minAltitude, sizeof(this->loggerData.minAltitude));
  staticMemoryWrite(this->staticMemory, address, &this->loggerData.maxAltitude, sizeof(this->loggerData.maxAltitude));
}

void GPSLogger::readLoggerData() {
  uint8_t dataStatus = 0;
  int address = kLoggerDataStatusAddress;
  staticMemoryRead(this->staticMemory, address, &dataStatus, sizeof(uint8_t));
  if (dataStatus!=kLoggerDataAvailable) {
    return;
  }
  address = kLoggerDataAddress;
  staticMemoryRead(this->staticMemory, address, &this->loggerData.distanceTravelled, sizeof(this->loggerData.distanceTravelled));
  staticMemoryRead(this->staticMemory, address, &this->loggerData.timeInMotion, sizeof(this->loggerData.timeInMotion));
  staticMemoryRead(this->staticMemory, address, &this->loggerData.maxSpeed, sizeof(this->loggerData.maxSpeed));
  staticMemoryRead(this->staticMemory, address, &this->loggerData.minAltitude, sizeof(this->loggerData.minAltitude));
  staticMemoryRead(this->staticMemory, address, &this->loggerData.maxAltitude, sizeof(this->loggerData.maxAltitude));
  this->loggerData.distanceTravelledScaled = this->loggerData.distanceTravelled / 100;
  this->loggerData.tripDistanceScaled = this->loggerData.tripDistance / 100;

}


uint32_t GPSLogger::dist(float lat1, float lng1, float lat2, float lng2) {
  float dist_calc=0;
  float dist_calc2=0;
  float diflat=0;
  float diflon=0;

  diflat=radians(lat2-lat1);
  lat1=radians(lat1);
  lat2=radians(lat2);
  diflon=radians((lng2)-(lng1));

  dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
  dist_calc2= cos(lat1);
  dist_calc2*=cos(lat2);
  dist_calc2*=sin(diflon/2.0);
  dist_calc2*=sin(diflon/2.0);
  dist_calc +=dist_calc2;

  dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

  dist_calc*=6371000.0; //Converting to meters
  return dist_calc;
}

const LoggerData& GPSLogger::getLoggerData() {
  return this->loggerData;
}

void GPSLogger::setDistanceTravelled(long dist) {
  this->loggerData.distanceTravelled = dist;
  this->loggerData.distanceTravelledScaled = dist / 100;
  this->writeLoggerData();
}
