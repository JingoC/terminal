#define _POSIX_C_SOURCE 199309L
#include "Time.h"
#include <stdio.h>
#include <time.h>

long long TimeUs(void) {
	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    long long t_us = t.tv_sec * 1000000LL + t.tv_nsec / 1000LL;
    return t_us;
}

long long TimeMs(void) {
	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    long long t_ms = t.tv_sec * 1000LL + t.tv_nsec / 1000000LL;
    return t_ms;
}
