#ifndef SLEEP_COMMON
#define SLEEP_COMMON

#include "config.h"

#ifdef RD_PLATFORM_WIN32
    #include <Windows.h>
    #define sleep_ms(x) Sleep(x)
#elif defined RD_PLATFORM_LINUX
    #include <unistd.h>
    #define sleep_ms(x) usleep(x * 1000)
#endif

#endif // SLEEP_COMMON
