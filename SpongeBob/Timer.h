#ifndef SPONGEBOB_TIMER_H
#define SPONGEBOB_TIMER_H

#include <stdint.h>
#include "CallBack.h"

using TimeStamp = int64_t;

class Timer {
   public:
    Timer(TimeStamp expiration, CallBack cb, bool repeat)
        : expiration_(expiration), callBack_(std::move(cb)), repeat_(repeat) {}

    TimeStamp getExpiration() const { return expiration_; }

    bool isRepeat() const { return repeat_; }
   private:
    // Timer开始时间
    TimeStamp expiration_;

    // 回调函数
    CallBack callBack_;

    // 启动间隔
    double interval_;
};

#endif