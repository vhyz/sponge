#include "EventLoopThreadPool.h"

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