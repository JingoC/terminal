#include "cli_time.h"

CLI_Time_s _def_time;

/// \brief Получить время в TTime из милисекунд
/// \param {uint32_t} msec - время в милисекундах
/// \return {TTime} - время
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

/// \brief Получить время в TTime из секунд
/// \param {uint32_t} sec - время в секундах
/// \return {TTime} - время
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

/// \brief Получить время в секундах из структуры
/// \param {TTime*} t - время
/// \return {uint32_t} - время в секундах
inline uint32_t CLI_GetTime(CLI_Time_s* t)
{
	uint32_t sec = t->hour * 3600 + t->minute * 60 + t->second;
	return sec;
}

/// \brief Получить время в милисекундах из структуры
/// \param {TTime*} t - время
/// \return {uint32_t} - время в милисекундах
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
