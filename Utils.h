#ifndef __UTILS_H__

#define __UTILS_H__

#include <stdint.h>

namespace utils {

void adjustdate(int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, int8_t hourdiff);

void adjustdst(int* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, const uint8_t dstStartMonth, const uint8_t dstStartDay, const uint8_t dstEndMonth, const uint8_t dstEndDay, const int8_t offsetToUTC, const int8_t dstOffsetToUTC);

}


#endif