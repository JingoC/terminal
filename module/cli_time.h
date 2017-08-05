#ifndef _TERMINAL_TIME_H_
#define _TERMINAL_TIME_H_

#include "terminal_config.h"

typedef struct{
	uint32_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t msec;
}CLI_Time_s;

CLI_Time_s CLI_GenerateTimeMSec(uint32_t msec);
CLI_Time_s CLI_GenerateTime(uint32_t sec);

uint32_t CLI_GetTime(CLI_Time_s* t);
uint32_t CLI_GetTimeMSec(CLI_Time_s* t);

void CLI_SetTimeMSec(CLI_Time_s* t, uint32_t msec);
void CLI_SetTimeSec(CLI_Time_s* t, uint32_t sec);
void CLI_SetTime(CLI_Time_s* t, uint32_t h , uint8_t m , uint8_t s);

#endif // _TERMINAL_TIME_H_
