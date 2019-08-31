#include "EventLoopThread.h"

namespace SpongeBob {

EventLoopThread::EventLoopThread() : thread_(), loop_(nullptr) {}

EventLoopThread::~EventLoopThread() {
    // 停止IO线程运行
    loop_->quit();

    // 等待线程结束
    thread_.join();
}

void EventLoopThread::start() {
    // 成员函数需要传递this指针
    thread_ = std::thread(&EventLoopThread::threadFunc, this);

    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == nullptr) condi_.wait(lock);
    }
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = &loop;
        condi_.notify_one();
    }
    loop_->loop();
}

}  // namespace SpongeBob