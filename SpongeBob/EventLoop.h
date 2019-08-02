#ifndef SPONGEBOB_EVENTLOOP_H
#define SPONGEBOB_EVENTLOOP_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "Channel.h"
#include "Poller.h"

class EventLoop {
   public:
    using Functor = std::function<void()>;

    EventLoop();

    void quit() { quit_ = true; }

    void loop();

    void updateChannel(Channel* channle) { poller_.updateChannel(channle); }

    void addChannel(Channel* channel) { poller_.addChannel(channel); }

    void deleteChannle(Channel* channle) { poller_.deleteChannel(channle); }

    void wakeUp();

    void addTask(Functor functor);

    void runTasks();

    void readWakeUpHandle();

    bool runInLoop();

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
};

#endif