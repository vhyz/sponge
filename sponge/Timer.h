#pragma once

#include <stdint.h>
#include "CallBack.h"

namespace sponge {

const int64_t kMicroSecondsPerSecond = 1000 * 1000;

using TimeStamp = int64_t;

TimeStamp getNowTimeStamp();
TimeStamp addTime(TimeStamp time, double seconds);

class Timer {
   public:
    Timer(TimeStamp expiration, CallBack cb, double interval)
        : expiration_(expiration),
          callBack_(std::move(cb)),
          interval_(interval) {}

    TimeStamp getExpiration() const { return expiration_; }

    double getInterval() const { return interval_; }

    void run() { callBack_(); }

    void restart(TimeStamp now) { expiration_ = addTime(now, interval_); }

    bool repeat() const { return interval_ > 0; }

   private:
    // Timer开始时间
    TimeStamp expiration_;

    // 回调函数
    CallBack callBack_;

    // 反复进行的间隔
    double interval_;
};

using TimerId = std::weak_ptr<Timer>;

}  // namespace sponge
