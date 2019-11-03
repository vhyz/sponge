#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "CallBack.h"
#include "Event.h"
#include "Poller.h"
#include "Timer.h"
#include "TimerManager.h"
#include "noncopyable.h"

namespace sponge {

class EventLoop : public noncopyable {
   public:
    using Functor = std::function<void()>;

    EventLoop();

    ~EventLoop();

    void quit();

    void loop();

    void updateEvent(Event* event) { poller_.updateEvent(event); }

    void addEvent(Event* event) { poller_.addEvent(event); }

    void removeEvent(Event* event) { poller_.removeEvent(event); }

    void runTasks();

    void runInLoop(const Functor& functor);
    void runInLoop(Functor&& functor);

    void queueInLoop(const Functor& functor);
    void queueInLoop(Functor&& functor);

    TimerId runAfter(double delay, CallBack cb);

    TimerId runEvery(double interval, CallBack cb);

    void cancel(TimerId timerId);

    bool isInLoopThread() const {
        return std::this_thread::get_id() == threadId_;
    }

   private:
    Poller poller_;

    bool quit_;

    std::thread::id threadId_;

    std::mutex mutex_;

    std::vector<Functor> taskList_;

    class WakeUpEvent;

    // 监听唤醒读事件
    std::unique_ptr<WakeUpEvent> wakeUpEvent_;

    TimerManager timerManager_;
};

}  // namespace sponge
