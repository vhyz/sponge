#pragma once

#include <memory>
#include <thread>
#include <vector>
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "noncopyable.h"

namespace sponge {

class EventLoopThreadPool : public noncopyable {
   public:
    EventLoopThreadPool(EventLoop* mainLoop, int numThread);

    ~EventLoopThreadPool() = default;

    // 由主线程运行，得到下一个loop
    EventLoop* getNextLoop();

    //启动线程池
    void start();

    int getNumThread() const { return numThread_; }

   private:
    std::vector<std::unique_ptr<EventLoopThread>> threadList_;

    // 用于accept的socket fd
    EventLoop* mainLoop_;

    // 线程数量
    int numThread_;

    // 下一个线程的索引
    int next_;
};

}  // namespace sponge
