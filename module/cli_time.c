#include "cli_time.h"

CLI_Time_s _def_time;

/// \brief Generate CLI_Time_s struct from milliseconds
CLI_Time_s CLI_GenerateTimeMSec(uint32_t msec)
{
    CLI_Time_s res;
	res.msec = msec % 1000;
	uint32_t s = msec / 1000;
	res.second = s % 60;
	uint32_t m = s / 60;
	res.minute = m % 60;
	uint32_t h = s / 3600;
	res.hour = h;// % 24;

	return res;
}

/// \brief Generate CLI_Time_s struct from seconds
CLI_Time_s CLI_GenerateTime(uint32_t sec)
{
    CLI_Time_s res;
	res.msec = 0;
	res.second = sec % 60;
	uint32_t m = sec / 60;
	res.minute = m % 60;
	uint32_t h = sec / 3600;
	res.hour = h;// % 24;

	return res;
}

/// \brief Get seconds from CLI_Time_s
inline uint32_t CLI_GetTime(CLI_Time_s* t)
{
	uint32_t sec = t->hour * 3600 + t->minute * 60 + t->second;
	return sec;
}

/// \brief Get milliseconds from CLI_Time_s
inline uint32_t CLI_GetTimeMSec(CLI_Time_s* t)
{
	uint32_t sec = t->hour * 3600000 + t->minute * 60000 + t->second * 1000 + t->msec;
	return sec;
}

inline void CLI_SetTimeSec(CLI_Time_s* t, uint32_t sec)
{
    *t = CLI_GenerateTime(sec);
}

inline void CLI_SetTimeMSec(CLI_Time_s* t, uint32_t msec)
{
    *t = CLI_GenerateTimeMSec(msec);
}

inline void CLI_SetTime(CLI_Time_s* t, uint32_t h , uint8_t m , uint8_t s)
{
	t->hour = h;// % 24;
	t->minute = m % 60;
	t->second = s % 60;
}
