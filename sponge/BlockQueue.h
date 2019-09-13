#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include "CallBack.h"

namespace sponge {

class BlockQueue {
   public:
    void put(CallBack cb);

    CallBack get();

    size_t size() const;

   private:
    mutable std::mutex mutex_;

    std::condition_variable condition_;

    std::queue<CallBack> taskQueue_;
};

}  // namespace sponge
