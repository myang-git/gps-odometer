#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

#include <Wire.h>
#include <SPI.h>
#include <SSD1306_text.h>
#include <ssdfont.h>

#include "GPS.h"
#include "LoggerData.h"
#include "Utils.h"


/*
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
*/
#define SSD1306_LCDWIDTH    128
#define SSD1306_LCDHEIGHT   64

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define kFontWidth                 5
#define kFontHeight                7
#define kSpeedFontSize             4
#define kSpeedFontSizeSm           2
#define kSpeedUnitFontSize         1
#define kTimeFontSize              1
#define kOdoFontSize               2
#define kOdoLastDigitFontSize      1
#define kTripOdoFontSize           2
#define kTripOdoLastDigitFontSize  1
#define kAltitudeFontSize          1
#define kHomeDistFontSize          1
#define kStatusFontSize            1

#define kCharSpacing        1

#define kMarginTop          0
#define kMarginBottom       0
#define kMarginLeft         0
#define kMarginRight        0
#define kStatusBarX         kMarginLeft
#define kStatusBarY         kMarginTop
#define kStatusBarHeight    2
#define kMetricsTop         (kStatusBarY + kStatusBarHeight)
#define kTwoDigitSpeedX     kMarginLeft
#define kOneDigitSpeedX     (kMarginLeft + kFontWidth * kSpeedFontSize + kCharSpacing)
#define kSpeedY             kMetricsTop
#define kSpeedFracX         (kMarginLeft + kFontWidth * kSpeedFontSize * 2 + kCharSpacing * kSpeedFontSize * 2)
#define kSpeedUnitX         (kMarginLeft + kFontWidth * kSpeedFontSize * 2 + kCharSpacing * kSpeedFontSize * 2)
#define kSpeedUnitY         (kMetricsTop + kSpeedFontSize - 1)

#define kTripOdoX           (SSD1306_LCDWIDTH - kMarginRight - kFontWidth * kTripOdoFontSize * 5 - kCharSpacing * 4)
#define kTripOdoLastDigitX  (kTripOdoX + kFontWidth * kTripOdoFontSize * 4 + kCharSpacing * 5)
#define kTripOdoY           kMetricsTop

#define kOdoX               (SSD1306_LCDWIDTH - kMarginRight - kFontWidth * kOdoFontSize * 5 - kCharSpacing * 4)
#define kOdoLastDigitX      (kOdoX + kFontWidth * kOdoFontSize * 4 + kCharSpacing * 5)
#define kOdoY               (kMetricsTop + kTripOdoFontSize)

#define kTimeX              (SSD1306_LCDWIDTH - kMarginRight - kFontWidth * kTimeFontSize * 5 - kCharSpacing * 4)
#define kTimeY              kStatusBarY

#define kAltitudeLabelX     kMarginLeft
#define kAltitudeX          (kAltitudeLabelX + kFontWidth * kAltitudeFontSize * 4 + kCharSpacing * 3)
#define kAltitudeY          (kMetricsTop + kSpeedFontSize + 1)

#define kHomeDistLabelX     (SSD1306_LCDWIDTH - kMarginRight - (kFontWidth * kHomeDistFontSize * 7 + kCharSpacing * 6) - (kFontWidth * kHomeDistFontSize * 2 + kCharSpacing * 2))
#define kHomeDistX          (SSD1306_LCDWIDTH - kMarginRight - (kFontWidth * kHomeDistFontSize * 7 + kCharSpacing * 6))

//#define kHomeDistLabelX     kMarginLeft
//#define kHomeDistX          (kHomeDistLabelX + kFontWidth * kHomeDistFontSize * 2 + kCharSpacing * 2)
#define kHomeDistY          (kMetricsTop + kSpeedFontSize + 2)

#define kGPSStatusX         kStatusBarX
#define kGPSStatusY         kStatusBarY

#define kGPSSignalX         (kStatusBarX + kFontWidth * kStatusFontSize + kCharSpacing)
#define kGPSSignalY         kStatusBarY

#define kSplashBarFontSize  2
#define kSplashBarSegments  6
#define kSplashBarSpacing   2
#define kSplashBarSegWidth  (kFontWidth * kSplashBarFontSize)
#define kSplashBarX         ((SSD1306_LCDWIDTH - (kSplashBarSegWidth * kSplashBarSegments + (kSplashBarSpacing * (kSplashBarSegments - 1)))) / 2)
#define kSplashBarY         4

#define WHITE               0xFF
#define BLACK               0x00

static const char BLOCK_CHAR = (char)124;
static const char SHORT_BLOCK_CHAR = (char)125;
static const char SHORT_EMPTY_BLOCK_CHAR = (char)126;
static const char LOCATION_CHAR = (char)127;

class Dashboard {
private:
  uint32_t lastSplashTime;
  byte splashState;
  SSD1306_text* display;
  bool gpsfix;
  uint32_t lastGPSStatusTime;
  void renderTime(GPS& gps);
  void renderSpeed(GPS& gps);
  void renderTripDistance(const LoggerData& data);
  void renderDistanceTravelled(const LoggerData& data, GPS& gps);
  void renderDistanceFromHome(const LoggerData& data);
  void renderAltitude(GPS& gps);
  void renderGPSSignal(GPS& gps);
  void renderGPSStatus(GPS& gps);
  void drawSplashBarSegment(uint8_t segment, uint8_t ch);
  void resetSplashBar();
  void adjustDST(int* year, byte* month, byte* day, byte* hour, byte* minute);

public:
  Dashboard();
  Dashboard(SSD1306_text* display);
  ~Dashboard();
  void begin();
  void splash();
  void dump(const LoggerData& data, GPS& gps);
  void render(const LoggerData& data, GPS& gps);
};


#endif
