#include "TimeStamp.h"
#include <sys/time.h>


TimeStamp now() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * kMicroSecondsPerSecond +
           tv.tv_usec;
}

TimeStamp addTime(TimeStamp time, double seconds) {
    return TimeStamp(time +
                     static_cast<int64_t>(seconds * kMicroSecondsPerSecond));
}