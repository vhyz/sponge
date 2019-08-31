#ifndef SPONGEBOB_EVENTLOOPTHREAD_H
#define SPONGEBOB_EVENTLOOPTHREAD_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include "EventLoop.h"

namespace SpongeBob {

class EventLoopThread {
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

}  // namespace SpongeBob
#endif