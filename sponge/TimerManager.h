#pragma once

#include <memory>
#include <set>
#include <utility>
#include <vector>
#include "Event.h"
#include "Timer.h"

namespace sponge {

class EventLoop;

class TimerManager {
   private:
    using Entry = std::pair<TimeStamp, std::shared_ptr<Timer>>;

   public:
    TimerManager(EventLoop* loop);

    ~TimerManager();

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

    Event event_;

    // 定时器最近任务发生的时间
    TimeStamp nextExpiration_;
};

}  // namespace sponge
