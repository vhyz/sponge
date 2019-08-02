#ifndef SPONGEBOB_EVENTLOOPTHREADPOOL_H
#define SPONGEBOB_EVENTLOOPTHREADPOOL_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "EventLoop.h"

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

class EventLoopThreadPool {
   public:
    EventLoopThreadPool(EventLoop* mainLoop, int numThread);

    ~EventLoopThreadPool() = default;

    // 由主线程运行，得到下一个loop
    EventLoop* getNextLoop();

    //启动线程池
    void start();

   private:
    std::vector<std::unique_ptr<EventLoopThread>> threadList_;

    // 用于accept的socket fd
    EventLoop* mainLoop_;
    
    // 线程数量
    int numThread_;

    // 下一个线程的索引
    int next_;
};
#endif