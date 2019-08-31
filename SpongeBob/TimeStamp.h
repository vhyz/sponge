#ifndef SPONGEBOB_TIMESTAMP_H
#define SPONGEBOB_TIMESTAMP_H

#include <stdint.h>

namespace SpongeBob {

const int64_t kMicroSecondsPerSecond = 1000 * 1000;

using TimeStamp = int64_t;

TimeStamp getNowTimeStamp();
TimeStamp addTime(TimeStamp time, double seconds);

}  // namespace SpongeBob
#endif