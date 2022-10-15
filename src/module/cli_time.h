#ifndef _TERMINAL_TIME_H_
#define _TERMINAL_TIME_H_

#include "terminal_config.h"

#define CLI_GetMs()			((float) CLI_GetUs() / 1000)	// System time in ms

void CLI_DelayMs(uint32_t ms);
void CLI_DelayUs(uint32_t us);

typedef struct{
	uint32_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t msec;
}CLI_Time_s;

void CLI_SetBaseTimeFromMs(uint32_t ms);
void CLI_SetBaseTimeFromHMS(uint32_t h, uint32_t m, uint32_t s);

CLI_Time_s CLI_GetFormatTimeByMs(uint32_t msec);
CLI_Time_s CLI_GetFormatLastTimeByMs(uint32_t msec);

#endif // _TERMINAL_TIME_H_
