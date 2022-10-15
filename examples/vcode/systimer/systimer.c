#include "systimer.h"

#include <windows.h>

static struct
{
	float startTimeMs;
}STMR;

float _GetCurrentMs()
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	return (time.wSecond * 1000) + time.wMilliseconds;
}

void STMR_Init()
{
	STMR.startTimeMs = _GetCurrentMs();
}

float STMR_GetMs()
{
	return _GetCurrentMs() - STMR.startTimeMs;
}

float STMR_GetUs()
{
	return STMR_GetMs() * 1000;
}
