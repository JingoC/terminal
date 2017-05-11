#include "terminal_time.h"

TTime _def_time;

/// \brief Получить время в TTime из милисекунд
/// \param {uint32_t} msec - время в милисекундах
/// \return {TTime} - время
TTime generateTimeMSec(uint32_t msec)
{
	TTime res;
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
TTime generateTime(uint32_t sec)
{
	TTime res;
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
uint32_t inline getTime(TTime* t)
{
	uint32_t sec = t->hour * 3600 + t->minute * 60 + t->second;
	return sec;
}

/// \brief Получить время в милисекундах из структуры
/// \param {TTime*} t - время
/// \return {uint32_t} - время в милисекундах
uint32_t inline getTimeMSec(TTime* t)
{
	uint32_t sec = t->hour * 3600000 + t->minute * 60000 + t->second * 1000 + t->msec;
	return sec;
}

void inline setTimeSec(TTime* t, uint32_t sec)
{
	*t = generateTime(sec);
}

void inline setTimeMSec(TTime* t, uint32_t msec)
{
	*t = generateTimeMSec(msec);
}

void inline setTime(TTime* t, uint32_t h , uint8_t m , uint8_t s)
{
	t->hour = h;// % 24;
	t->minute = m % 60;
	t->second = s % 60;
}
