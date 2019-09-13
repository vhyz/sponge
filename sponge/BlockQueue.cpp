#include "BlockQueue.h"

namespace sponge {

void BlockQueue::put(CallBack cb) {
    std::lock_guard<std::mutex> lock(mutex_);
    taskQueue_.push(std::move(cb));
    condition_.notify_one();
}

CallBack BlockQueue::get() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (taskQueue_.empty()) {
        condition_.wait(lock);
    }
    CallBack ret = std::move(taskQueue_.front());
    taskQueue_.pop();
    return ret;
}

size_t BlockQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return taskQueue_.size();
}

}  // namespace sponge