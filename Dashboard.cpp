#include "Dashboard.h"

//Adafruit_SSD1306 gDisplay(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define TESTMODE

Dashboard::Dashboard() {
  //this->display = &gDisplay;
}

Dashboard::Dashboard(SSD1306_text* display) {
  this->display = display;
  this->splashState = 0;
  this->gpsfix = false;
}

Dashboard::~Dashboard() {
}

void Dashboard::begin() {
  display->init();
  display->clear();
  Serial.println(F("dashboard ready"));
}

void Dashboard::renderSpeed(GPS& gps) {
  //draw speed

  this->display->setTextSize(kSpeedFontSize, kCharSpacing);
  this->display->setCursor(kSpeedY, gps.speed() >= 10 ? kTwoDigitSpeedX : kOneDigitSpeedX);
  byte integer = (int)(gps.speed());
  this->display->print(min(integer, 99));

  byte frac = round((gps.speed() - (long)gps.speed()) * 10);
  this->display->setTextSize(kSpeedFontSizeSm, kCharSpacing);
  this->display->setCursor(kSpeedY, kSpeedFracX);
  this->display->print(frac);

  this->display->setTextSize(kSpeedUnitFontSize, kCharSpacing);
  this->display->setCursor(kSpeedUnitY, kSpeedUnitX);
  this->display->print(F("kph"));

}

void Dashboard::renderTripDistance(const LoggerData& data) {
  char buf[6] = {0, 0, 0, 0, 0, 0};
  if (data.tripDistance<10000) {
    sprintf(buf, "%04d", data.tripDistance);
    this->display->setTextSize(kTripOdoFontSize, kCharSpacing);
    this->display->setCursor(kTripOdoY, kTripOdoX);
    this->display->print(buf);
    this->display->setTextSize(kTripOdoLastDigitFontSize, kCharSpacing);
    this->display->setCursor(kTripOdoY, kTripOdoLastDigitX);
    this->display->print("m");
  }
  else {
    sprintf(buf, "%04d", data.tripDistanceScaled / 10);
    this->display->setTextSize(kTripOdoFontSize, kCharSpacing);
    this->display->setCursor(kTripOdoY, kTripOdoX);
    this->display->print(buf);
    buf[1] = 0;
    sprintf(buf, "%d", data.tripDistanceScaled % 10);
    this->display->setTextSize(kTripOdoLastDigitFontSize, kCharSpacing);
    this->display->setCursor(kTripOdoY, kTripOdoLastDigitX);
    this->display->print(buf);
  }
}

void Dashboard::renderDistanceTravelled(const LoggerData& data, GPS& gps) {
  char buf[6] = {0, 0, 0, 0, 0, 0};
  sprintf(buf, "%04d", data.distanceTravelledScaled / 10);
  this->display->setTextSize(kOdoFontSize, kCharSpacing);
  this->display->setCursor(kOdoY, kOdoX);
  this->display->print(buf);
  buf[1] = 0;
  sprintf(buf, "%d", data.distanceTravelledScaled % 10);
  this->display->setTextSize(kOdoLastDigitFontSize, kCharSpacing);
  this->display->setCursor(kOdoY, kOdoLastDigitX);
  this->display->print(buf);
}

void Dashboard::renderAltitude(GPS& gps) {
  char buf[7] = {0, 0, 0, 0, 0, 0, 0};
  int altitude = (int)(min(9999, gps.altitude()));
  sprintf(buf, "%4dm", altitude);
  this->display->setTextSize(kAltitudeFontSize, kCharSpacing);
  this->display->setCursor(kAltitudeY, kAltitudeLabelX);
  this->display->print(F("Alt:"));
  this->display->setCursor(kAltitudeY, kAltitudeX);
  this->display->print(buf);
}

void Dashboard::renderDistanceFromHome(const LoggerData& data) {
  char buf[8];
  memset(buf, 0, sizeof(buf));
  buf[0] = data.homeLatDir;
  buf[1] = data.homeLngDir;
  this->display->setTextSize(kHomeDistFontSize, kCharSpacing);
  this->display->setCursor(kHomeDistY, kHomeDistLabelX);
  this->display->print(buf);
  if (data.distanceFromHome<=1500) {
    sprintf(buf, "%6dm", data.distanceFromHome);
  }
  else {
    float km = min(999, data.distanceFromHome / 1000.0);
    char fpbuf[6] = {0,0,0,0,0,0};
    dtostrf(km, 5, 1, fpbuf);
    sprintf(buf, "%skm", fpbuf);
  }
  this->display->setCursor(kHomeDistY, kHomeDistX);
  this->display->print(buf);
}

void Dashboard::renderTime(GPS& gps) {
  char buf[6];
  memset(buf, 0, sizeof(buf));
  int year;
  byte month, day, hour, minute, second;
  gps.datetime(&year, &month, &day, &hour, &minute, &second);
  if (hour>=24 || hour<0) {
    hour = 0;
  }
  if (minute>=60 || minute <0) {
    minute = 0;
  }
  utils::adjustdst(&year, &month, &day, &hour, &minute, 3, 8, 11, 1, -5, -4);
  sprintf(buf, "%02d:%02d", hour, minute);
  this->display->setTextSize(kTimeFontSize, kCharSpacing);
  this->display->setCursor(kTimeY, kTimeX);
  this->display->print(buf);
}

void Dashboard::render(const LoggerData& data, GPS& gps) {
  renderSpeed(gps);
  renderDistanceTravelled(data, gps);
  renderTripDistance(data);
  renderDistanceFromHome(data);
  renderTime(gps);
  renderAltitude(gps);
  renderGPSSignal(gps);
  renderGPSStatus(gps);
}

void Dashboard::drawSplashBarSegment(uint8_t segment, uint8_t ch = BLOCK_CHAR) {
  this->display->setCursor(kSplashBarY, kSplashBarX + (kSplashBarSegWidth + kSplashBarSpacing) * segment);
  this->display->setTextSize(kSplashBarFontSize, kCharSpacing);
  this->display->write(ch);
}

void Dashboard::resetSplashBar() {
  for (int i=0;i<kSplashBarSegments;i++) {
    drawSplashBarSegment(i, ' ');
  }
}

void Dashboard::renderGPSSignal(GPS& gps) {
  float hdop = gps.hdop();
  char buf[6] = {SHORT_BLOCK_CHAR, SHORT_BLOCK_CHAR, SHORT_BLOCK_CHAR, SHORT_BLOCK_CHAR, SHORT_BLOCK_CHAR, 0};
  int idx = 4;
  for (int idx = 4; idx>=0; idx--) {
    int level = (5 - idx) * 200;
    if (hdop>level) {
      buf[idx] = SHORT_EMPTY_BLOCK_CHAR;
    }
  }

  this->display->setTextSize(kStatusFontSize, kCharSpacing);
  this->display->setCursor(kGPSSignalY, kGPSSignalX);
  this->display->print(buf);
}

void Dashboard::renderGPSStatus(GPS& gps) {
  bool statusChanged = false;
  uint32_t now = millis();
  if (gps.fix()) {
    if (!this->gpsfix) {
      statusChanged = true;
    }
    this->gpsfix = true;
  }
  else {
    if (this->lastGPSStatusTime==0) {
      this->lastGPSStatusTime = now;
    }
    else if (now - this->lastGPSStatusTime > 1000) {
      this->gpsfix = !this->gpsfix;
      this->lastGPSStatusTime = now;
      statusChanged = true;
    }
  }
  if (statusChanged) {
    this->display->setTextSize(kStatusFontSize, kCharSpacing);
    this->display->setCursor(kGPSStatusY, kGPSStatusX);
    this->display->write(this->gpsfix ? '*' : ' ');
  }
}

void Dashboard::dump(const LoggerData& data, GPS& gps) {
  float lat, lng;
  float speed, altitude, hdop;
  int year;
  uint8_t month, day, hour, minute, second;
  gps.query(&lat, &lng, &speed, &altitude, &hdop, &year, &month, &day, &hour, &minute, &second);
  uint8_t row = 0;
  this->display->clear();
  this->display->setCursor(row++, 0);
  this->display->print((long)(lat*100000.0));
  this->display->print(',');
  this->display->print((long)(lng*100000.0));

  this->display->setCursor(row++, 0);
  this->display->print((long)(data.lastLat * 100000.0));
  this->display->print(F(","));
  this->display->print((long)(data.lastLng * 100000.0));

  this->display->setCursor(row++, 0);
  this->display->print(F("s:"));
  this->display->print(speed);
  this->display->print(F(","));
  this->display->print(F("td:"));
  this->display->print(data.tripDistance);
  this->display->print(F(","));
  this->display->print(F("ld:"));
  this->display->print(data.lastDistance);

  this->display->setCursor(row++, 0);
  this->display->print(hdop, 0);
  this->display->print(F(","));
  this->display->print(gps.fix() ? "FIX" : "NFX");
  this->display->print(F(","));
  this->display->print(F("lg:"));
  this->display->print(data.logged ? "Y" : "N");

  this->display->setCursor(row++, 0);
  this->display->print(F("lav:"));
  this->display->print(data.lastLocationAvailable ? "Y" : "N");
  this->display->print(F(","));
  this->display->print(F("k:"));
  this->display->print(data.logCounter);
/*
  this->display->setCursor(row++, 0);
  this->display->print(F("alt:"));
  this->display->print(altitude);
*/

/*
  this->display->setCursor(row++, 0);
  this->display->print(F("dt:"));
  this->display->print(year);
  this->display->print(F(" "));
  this->display->print(month);
  this->display->print(F(" "));
  this->display->print(day);
  this->display->print(F(" "));
  this->display->print(hour);
  this->display->print(F(" "));
  this->display->print(minute);
  this->display->print(F(" "));
  this->display->print(second);
*/


}

