#pragma once

#include <stdint.h>

namespace sponge {

const int64_t kMicroSecondsPerSecond = 1000 * 1000;

using TimeStamp = int64_t;

TimeStamp getNowTimeStamp();
TimeStamp addTime(TimeStamp time, double seconds);

}  // namespace sponge
