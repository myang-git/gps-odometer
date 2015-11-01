#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <SSD1306_text.h>
#include <ssdfont.h>
#include <Adafruit_FRAM_I2C.h>

#include "SystemConfig.h"
#include "PinConfig.h"
#include "GPS.h"
#include "GPSLogger.h"
#include "Dashboard.h"

#define OLED_DC 6     //Atmega32u4 Breakout Pin D7
#define OLED_CS 9     //Atmega32u4 Breakout Pin B5
#define OLED_CLK 5    //Atmega32u4 Breakout Pin C6
#define OLED_MOSI 4   //Atmega32u4 Breakout Pin D4 (Pin on OLED labeled DATA)
#define OLED_RESET 8  //Atmega32u4 Breakout Pin B4

#define GPS_RX_PIN 3
#define GPS_TX_PIN 2


#define USETINYGPSPLUS

#ifdef USETINYGPS
#include "TinyGPSAdapter.h"
#endif

#ifdef USETINYGPSPLUS
#include "TinyGPSPlusAdapter.h"
#endif

#ifdef USEADAFRUITGPS
#include "AdafruitGPSAdapter.h"
#endif

#ifdef USENULLGPS
#include "NullGPS.h"
#endif


SoftwareSerial swserial(GPS_RX_PIN, GPS_TX_PIN);  //rx, tx
#ifdef USETINYGPS
TinyGPSAdapter adapter(&swserial);
#endif

#ifdef USETINYGPSPLUS
TinyGPSPlusAdapter adapter(&swserial);
#endif

#ifdef USEADAFRUITGPS
AdafruitGPSAdapter adapter(&swserial);
#endif

#ifdef USENULLGPS
NullGPS adapter;
#endif

Adafruit_FRAM_I2C staticMemory;
GPS* gps = &adapter;
GPSLogger gpsLogger(gps, &staticMemory);

SystemConfig systemConfig;
SystemConfig restartSystemConfig;

//SSD1306_text(int8_t data, int8_t clk, int8_t dc, int8_t rst, int8_t cs)
SSD1306_text display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Dashboard dashboard(&display);

void loadSystemConfig() {
  readSystemConfig(&staticMemory, systemConfig);
  restartSystemConfig = systemConfig;
  Serial.print("system mode: ");
  Serial.println(systemConfig.mode);
  restartSystemConfig.mode = kTestMode;
  writeSystemConfig(&staticMemory, restartSystemConfig);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("system initializing"));
  staticMemory.begin();
  Serial.println(F("static memory ready"));
  loadSystemConfig();
  gps->begin();
  gpsLogger.begin();
  dashboard.begin();
  Serial.println(F("system ready"));
}


uint32_t gpsLoggerTimer = millis();
uint32_t startTime = gpsLoggerTimer;
uint32_t now = 0;
bool savedRestartSystemConfig = false;
void loop() {
  volatile bool gpsHasFix = false;
  if (gpsLoggerTimer > millis()) {
    gpsLoggerTimer = millis();
  }

  gps->read();

  now = millis();
  if (now - gpsLoggerTimer > 500) { 
    Serial.println(F("beep"));
    if (gps->fix()) {
      gpsLogger.log();
    }
    else {
      gpsLogger.resetLastLocation();
    }
    gpsLoggerTimer = millis();
    if (systemConfig.mode==kTestMode) {
      dashboard.dump(gpsLogger.getLoggerData(), *gps);
    }
    else {
      dashboard.render(gpsLogger.getLoggerData(), *gps);
    }
  }

  if (now - startTime > 5000 && !savedRestartSystemConfig) {
    restartSystemConfig.mode = kNormalMode;
    writeSystemConfig(&staticMemory, restartSystemConfig);
    savedRestartSystemConfig = true;
  }
}
