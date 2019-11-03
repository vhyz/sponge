#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include "EventLoop.h"
#include"noncopyable.h"

namespace sponge {

class EventLoopThread: public noncopyable {
   public:
    EventLoopThread();

    ~EventLoopThread();

    void start();

    EventLoop* getEventLoop() const { return loop_; }

    void join() { thread_.join(); }

   private:
    std::thread thread_;

    EventLoop* loop_;

    std::mutex mutex_;

    std::condition_variable condi_;

    void threadFunc();
};

}  // namespace sponge
