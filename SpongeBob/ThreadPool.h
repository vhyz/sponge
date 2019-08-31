#ifndef SPONGEBOB_THREADPOOL_H
#define SPONGEBOB_THREADPOOL_H

#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "BlockQueue.h"
#include "CallBack.h"

namespace SpongeBob {

class ThreadPool {
   public:
    explicit ThreadPool(size_t threadPoolSize);

    ~ThreadPool();

    void start();

    void putTask(CallBack cb);

   private:
    void threadFunc();

    std::vector<std::thread> threadList_;

    BlockQueue taskQueue_;
};

}  // namespace SpongeBob

#endif