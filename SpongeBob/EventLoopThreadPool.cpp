#include "EventLoopThreadPool.h"

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

EventLoopThreadPool::EventLoopThreadPool(EventLoop* mainLoop, int numThread)
    : mainLoop_(mainLoop), numThread_(numThread), next_(0) {
    threadList_.resize(numThread_);

    for (size_t i = 0; i < threadList_.size(); ++i) {
        threadList_[i] = std::make_unique<EventLoopThread>();
    }
}

void EventLoopThreadPool::start() {
    for (auto& thread : threadList_) {
        thread->start();
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    if (numThread_ > 0) {
        // 返回策略为平均策略
        EventLoop* res = threadList_[next_]->getEventLoop();

        next_ = (next_ + 1) % numThread_;

        return res;
    } else {
        return mainLoop_;
    }
}