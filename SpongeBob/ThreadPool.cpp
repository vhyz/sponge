#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadPoolSize) {
    threadList_.reserve(threadPoolSize);
}

void ThreadPool::putTask(CallBack cb) { taskQueue_.put(std::move(cb)); }

void ThreadPool::start() {
    size_t capacity = threadList_.capacity();

    for (size_t i = 0; i < capacity; ++i) {
        threadList_.push_back(std::thread(&ThreadPool::threadFunc, this));
    }
}