#include "TimeStamp.h"
#include <sys/time.h>

const int64_t kMicroSecondsPerSecond = 1000 * 1000;

TimeStamp now() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * kMicroSecondsPerSecond +
           tv.tv_usec;
}