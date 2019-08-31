#include "ThreadPool.h"

namespace SpongeBob {

ThreadPool::ThreadPool(size_t threadPoolSize) {
    threadList_.reserve(threadPoolSize);
}

ThreadPool::~ThreadPool() {
    for (size_t i = 0; i < threadList_.size(); ++i) {
        threadList_[i].join();
    }
}

void ThreadPool::putTask(CallBack cb) { taskQueue_.put(std::move(cb)); }

void ThreadPool::start() {
    size_t capacity = threadList_.capacity();

    for (size_t i = 0; i < capacity; ++i) {
        threadList_.push_back(std::thread(&ThreadPool::threadFunc, this));
    }
}

void ThreadPool::threadFunc() {
    while (true) {
        CallBack task = taskQueue_.get();
        task();
    }
}

}  // namespace SpongeBob