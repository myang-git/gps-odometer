#include "Utils.h"

namespace utils {

void adjustdate(int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, int8_t hourdiff) {
  uint8_t days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (((*year % 4 == 0) && (*year % 100 != 0)) || (*year % 400)==0) {
    days[1] = 29;
  }

  int _year = *year;
  int8_t _month = *month;
  int8_t _day = *day;
  int8_t _hour = *hour;

  int8_t dayinc = 0;
  int8_t monthinc = 0;
  int8_t yearinc = 0;
  _hour = _hour + hourdiff;
  if (_hour>=24) {
    _hour = _hour - 24;
    dayinc = 1;
  }
  else if(_hour<0) {
    _hour = _hour + 24;
    dayinc = -1;
  }

  _day = _day + dayinc;
  if (_day>days[_month - 1]) {
    _day = 1;
    monthinc = 1;
  }
  else if(_day<1) {
    if (_month>0) {
      _day = days[_month - 1];
    }
    else {
      _day = days[11];
    }
    monthinc = -1;
  }

  _month = _month + monthinc;
  if (_month>12) {
    _month = 1;
    yearinc = 1;
  }
  else if(_month<1) {
    _month = 12;
    yearinc = -1;
  }
  *year = *year + yearinc;
  *month = _month;
  *day = _day;
  *hour = _hour;
}

void adjustdst(int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, const uint8_t dstStartMonth, const uint8_t dstStartDay, const uint8_t dstEndMonth, const uint8_t dstEndDay, const int8_t offsetToUTC, const int8_t dstOffsetToUTC) {
  if ((*month==dstStartMonth && *day>=dstStartDay) || (*month>dstStartMonth && *month<dstEndMonth)) {
    adjustdate(year, month, day, hour, minute, dstOffsetToUTC);
  }
  else {
    adjustdate(year, month, day, hour, minute, offsetToUTC);
  }
}


}

