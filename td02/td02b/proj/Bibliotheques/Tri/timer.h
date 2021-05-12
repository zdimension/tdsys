#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>
#include <windows.h>

#ifndef NANO_TO_SECONDS
#   define NANO_TO_SECONDS 1000000000L
#endif

LARGE_INTEGER timer_start();
long timer_end(LARGE_INTEGER start_time);

#endif