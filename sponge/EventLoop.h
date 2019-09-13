#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "CallBack.h"
#include "Channel.h"
#include "Poller.h"
#include "Timer.h"
#include"TimerManager.h"

namespace sponge {

class EventLoop {
   public:
    using Functor = std::function<void()>;

    EventLoop();

    void quit();

    void loop();

    void updateChannel(Channel* channle) { poller_.updateChannel(channle); }

    void addChannel(Channel* channel) { poller_.addChannel(channel); }

    void deleteChannle(Channel* channle) { poller_.deleteChannel(channle); }

    void wakeUp();

    void readWakeUpHandle();

    void runTasks();

    void runInLoop(Functor functor);

    void queueInLoop(Functor functor);

    TimerId runAfter(double delay, CallBack cb);

    TimerId runEvery(double interval, CallBack cb);

    void cancel(TimerId timerId);

    bool isInLoopThread() const {
        return std::this_thread::get_id() == threadId_;
    }

   private:
    Poller poller_;

    std::vector<Channel*> avtiveChannelList_;

    bool quit_;

    std::thread::id threadId_;

    std::mutex mutex_;

    std::vector<Functor> taskList_;

    // 用于唤醒线程的eventfd
    int wakeUpFd_;

    // 监听唤醒读事件
    Channel wakeChannel_;

    TimerManager timerManager_;
};

}  // namespace sponge
