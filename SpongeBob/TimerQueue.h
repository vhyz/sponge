#ifndef SPONGEBOB_TIMERQUEUE_H
#define SPONGEBOB_TIMERQUEUE_H

#include <memory>
#include <set>
#include <utility>
#include <vector>
#include "Channel.h"
#include "Timer.h"

using TimerId = std::weak_ptr<Timer>;

class TimerQueue {
   private:
    using Entry = std::pair<TimeStamp, std::shared_ptr<Timer>>;

   public:
    TimerQueue(EventLoop* loop);

    ~TimerQueue();

    TimerId addTimer(TimeStamp timeStamp, CallBack cb, double interval);

    void cancel(TimerId timerId);

   private:
    std::vector<Entry> getExpired(TimeStamp now);

    void addTimerInLoop(std::shared_ptr<Timer> timer);

    void cancelTimerInLoop(TimerId timerId);

    void resetTimerFd();

    void insert(std::shared_ptr<Timer> timer);

    // 运行之后更新定时器
    void update(const std::vector<Entry>& entries, TimeStamp now);

    // 定时器到期时回调函数
    void handleRead();

   private:
    std::set<Entry> timers_;

    EventLoop* loop_;

    int timerFd_;

    Channel channel_;

    // 定时器最近任务发生的时间
    TimeStamp nextExpiration_;
};

#endif