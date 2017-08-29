#include "cli_time.h"

uint32_t _def_time_ms;

/// \brief Generate CLI_Time_s struct from milliseconds
CLI_Time_s CLI_GetFormatTimeByMs(uint32_t msec)
{
    CLI_Time_s res;
	res.msec = msec % 1000;
	uint32_t s = msec / 1000;
	res.second = s % 60;
	uint32_t m = s / 60;
	res.minute = m % 60;
	uint32_t h = s / 3600;
	res.hour = h;

	return res;
}

inline void CLI_SetBaseTimeFromMs(uint32_t ms)
{
	_def_time_ms = ms;
}

inline void CLI_SetBaseTimeFromHMS(uint32_t h, uint32_t m, uint32_t s)
{
	_def_time_ms = h * 3600000 + m * 60000 + s * 1000;
}

inline CLI_Time_s CLI_GetFormatLastTimeByMs(uint32_t msec)
{
	return CLI_GetFormatTimeByMs(msec + _def_time_ms);
}

void CLI_DelayMs(uint32_t ms)
{
	uint32_t startMs = CLI_GetMs();
	while(((uint32_t)CLI_GetMs() - startMs) < (ms)) {}
}

void CLI_DelayUs(uint32_t us)
{
	uint32_t startUs = CLI_GetUs();
	while(((uint32_t)CLI_GetUs() - startUs) < (us)) {}
}
