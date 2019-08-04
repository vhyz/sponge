#ifndef SPONGEBOB_EVENTLOOPTHREADPOOL_H
#define SPONGEBOB_EVENTLOOPTHREADPOOL_H

#include <memory>
#include <thread>
#include <vector>
#include "EventLoop.h"
#include "EventLoopThread.h"

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