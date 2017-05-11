#ifndef _TERMINAL_TIME_H_
#define _TERMINAL_TIME_H_

#include "terminal_config.h"

typedef struct{
	uint32_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t msec;
}TTime;

TTime generateTimeMSec(uint32_t msec);
TTime generateTime(uint32_t sec);

uint32_t getTime(TTime* t);
uint32_t getTimeMSec(TTime* t);

void setTimeMSec(TTime* t, uint32_t msec);
void setTimeSec(TTime* t, uint32_t sec);
void setTime(TTime* t, uint32_t h , uint8_t m , uint8_t s);

#endif // _TERMINAL_TIME_H_
