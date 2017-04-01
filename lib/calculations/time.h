/*!
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef CAL_TIME_H
#define CAL_TIME_H

#include <pkmn/config.hpp>

#if defined(PKMN_PLATFORM_MACOS)
#include <mach/mach_time.h>
#include <unistd.h>

static inline unsigned long long cal_highres_timestamp() {
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);

    // Convert nanoseconds to microseconds
    return (mach_absolute_time() * (timebase_info.numer / timebase_info.denom)) / 1000;
}

static inline void cal_usleep(
    unsigned long long sleep_time
) {
    usleep(sleep_time);
}

#elif defined(PKMN_PLATFORM_WIN32) || defined(PKMN_PLATFORM_MINGW)
#include <windows.h>

static inline unsigned long long cal_highres_timestamp() {
    FILETIME filetime;
#if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8
    GetSystemTimePreciseAsFileTime(&filetime);
#else
    GetSystemTimeAsFileTime(&filetime);
#endif

    // Convert 100-nanosecond granularity to microseconds
    return *((unsigned long long*)&filetime) / 10;
}

static inline void cal_usleep(
    unsigned long long sleep_time
) {
    unsigned long long timestamp1 = cal_highres_timestamp();
    while((cal_highres_timestamp() - timestamp1) < sleep_time);
}

#else
#include <time.h>
#include <unistd.h>

static inline unsigned long long cal_highres_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    // Convert nanoseconds to microseconds
    return ((ts.tv_sec * 1e9) + ts.tv_nsec) / 1000;
}

static inline void cal_usleep(
    unsigned long long sleep_time
) {
    usleep(sleep_time);
}

#endif

#endif
