#include "EventLoop.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include "Logger.h"

namespace sponge {

int createEventFd() {
    int fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (fd < 0) {
        FATAL("createEventFd() error");
    }
    return fd;
}

class EventLoop::WakeUpEvent {
   public:
    WakeUpEvent() : fd_(createEventFd()), event_(fd_) {
        event_.enableRead();
        event_.setEventCallBack(
            std::bind(&WakeUpEvent::handleEvent, this, std::placeholders::_1));
    }

    ~WakeUpEvent() { ::close(fd_); }

    void handleEvent(uint32_t events) {
        if (events & Event::SPEV_READ) {
            handleRead();
        }
    }

    void wakeUp() {
        uint64_t one = 1;
        write(fd_, &one, sizeof(one));
    }

    Event* getEvent() { return &event_; }

   private:
    void handleRead() {
        char buf[sizeof(uint64_t) * 1024];
        while (true) {
            int cnt = read(fd_, buf, sizeof(buf));

            if (cnt == -1 || static_cast<size_t>(cnt) < sizeof(buf)) {
                break;
            }
        }
    }

   private:
    int fd_;

    Event event_;
};

EventLoop::EventLoop()
    : poller_(),
      quit_(false),
      threadId_(std::this_thread::get_id()),
      mutex_(),
      taskList_(),
      wakeUpEvent_(std::make_unique<WakeUpEvent>()),
      timerManager_(this) {
    this->addEvent(wakeUpEvent_->getEvent());
}

EventLoop::~EventLoop() { this->removeEvent(wakeUpEvent_->getEvent()); }

void EventLoop::quit() {
    quit_ = true;
    if (!isInLoopThread()) {
        wakeUpEvent_->wakeUp();
    }
}

void EventLoop::loop() {
    quit_ = false;
    while (!quit_) {
        poller_.poll(1000);

        // 每一次唤醒都要执行一下任务
        // 确保事件的变化
        runTasks();
    }
}

void EventLoop::runInLoop(Functor&& functor) {
    if (isInLoopThread()) {
        functor();
    } else {
        queueInLoop(std::move(functor));
    }
}

void EventLoop::runInLoop(const Functor& functor) {
    if (isInLoopThread()) {
        functor();
    } else {
        queueInLoop(functor);
    }
}

void EventLoop::queueInLoop(Functor&& functor) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        taskList_.push_back(std::move(functor));
    }

    wakeUpEvent_->wakeUp();
}

void EventLoop::queueInLoop(const Functor& functor) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        taskList_.push_back(functor);
    }

    wakeUpEvent_->wakeUp();
}

void EventLoop::runTasks() {
    std::vector<Functor> taskList;
    {
        std::lock_guard<std::mutex> guard(mutex_);
        taskList.swap(taskList_);
    }
    for (auto& task : taskList) {
        task();
    }
}

TimerId EventLoop::runAfter(double delay, CallBack cb) {
    TimeStamp when = addTime(getNowTimeStamp(), delay);
    return timerManager_.addTimer(when, std::move(cb), 0.0);
}

TimerId EventLoop::runEvery(double interval, CallBack cb) {
    TimeStamp when = addTime(getNowTimeStamp(), interval);
    return timerManager_.addTimer(when, std::move(cb), interval);
}

void EventLoop::cancel(TimerId timerId) {
    timerManager_.cancel(std::move(timerId));
}

}  // namespace sponge