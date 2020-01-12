#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "CallBack.h"
#include "Event.h"
#include "Poller.h"
#include "Timer.h"
#include "TimerManager.h"
#include "noncopyable.h"

namespace sponge {

class EventLoop : public noncopyable {
   public:
    using Functor = std::function<void()>;

    EventLoop();

    ~EventLoop();

    void quit();

    void loop();

    void updateEvent(Event* event) { poller_.updateEvent(event); }

    void addEvent(Event* event) { poller_.addEvent(event); }

    void removeEvent(Event* event) { poller_.removeEvent(event); }

    void runTasks();

    void runInLoop(const Functor& functor);
    void runInLoop(Functor&& functor);

    void queueInLoop(const Functor& functor);
    void queueInLoop(Functor&& functor);

    TimerId runAfter(double delay, CallBack cb);

    TimerId runEvery(double interval, CallBack cb);

    void cancel(TimerId timerId);

    bool isInLoopThread() const {
        return std::this_thread::get_id() == threadId_;
    }

   private:
    Poller poller_;

    bool quit_;

    std::thread::id threadId_;

    std::mutex mutex_;

    std::vector<Functor> taskList_;

    class WakeUpEvent;

    // 监听唤醒读事件
    std::unique_ptr<WakeUpEvent> wakeUpEvent_;

    std::vector<Event*> activeEventList_;

    TimerManager timerManager_;
};

class EventLoopThread : public noncopyable {
   public:
    EventLoopThread();

    ~EventLoopThread();

    void start();

    EventLoop* getEventLoop() const { return loop_; }

    void join() { thread_.join(); }

   private:
    std::thread thread_;

    EventLoop* loop_;

    std::mutex mutex_;

    std::condition_variable condi_;

    void threadFunc();
};

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
