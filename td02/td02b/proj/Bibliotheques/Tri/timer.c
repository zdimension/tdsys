#include "timer.h"
#include <windows.h>

// call this function to start a nanosecond-resolution timer
LARGE_INTEGER timer_start() {
    LARGE_INTEGER val;
    QueryPerformanceCounter(&val);
    return val;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(LARGE_INTEGER start_time)
{
    LARGE_INTEGER end, freq;
    QueryPerformanceCounter(&end);
    QueryPerformanceFrequency(&freq);
    return (end.QuadPart - start_time.QuadPart) * 1000000000L / freq.QuadPart;
}
