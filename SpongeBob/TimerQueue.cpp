#include "TimerQueue.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include "EventLoop.h"

namespace SpongeBob {

int create_timerfd() {
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

    if (fd < 0) {
        std::cout << "create_timerfd error" << std::endl;
        exit(1);
    }

    return fd;
}

TimerQueue::TimerQueue(EventLoop* loop)
    : loop_(loop), timerFd_(-1), channel_(), nextExpiration_(-1) {
    timerFd_ = create_timerfd();

    channel_.setFd(timerFd_);
    channel_.setEvents(EPOLLIN);
    channel_.setReadCallBack(std::bind(&TimerQueue::handleRead, this));

    loop_->addChannel(&channel_);
}

TimerQueue::~TimerQueue() {
    loop_->deleteChannle(&channel_);
    close(timerFd_);
}

TimerId TimerQueue::addTimer(TimeStamp when, CallBack cb, double interval) {
    auto timer = std::make_shared<Timer>(when, std::move(cb), interval);

    // 在移动timer前构造TimerId
    TimerId ret(timer);

    loop_->runInLoop(
        std::bind(&TimerQueue::addTimerInLoop, this, std::move(timer)));

    return ret;
}

void TimerQueue::cancel(TimerId timerId) {
    loop_->runInLoop(
        std::bind(&TimerQueue::cancelTimerInLoop, this, std::move(timerId)));
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeStamp now) {
    Entry entry(now + 1, std::shared_ptr<Timer>());

    auto end = timers_.lower_bound(entry);

    std::vector<Entry> ret(timers_.begin(), end);
    timers_.erase(timers_.begin(), end);

    return ret;
}

void TimerQueue::addTimerInLoop(std::shared_ptr<Timer> timer) {
    insert(std::move(timer));
    resetTimerFd();
}

void TimerQueue::cancelTimerInLoop(TimerId timerId) {
    if (auto sp = timerId.lock()) {
        Entry entry(sp->getExpiration(), sp);
        timers_.erase(entry);
    }
}

void TimerQueue::resetTimerFd() {
    assert(!timers_.empty());

    TimeStamp next = timers_.begin()->second->getExpiration();

    if (next < nextExpiration_ || nextExpiration_ < 0) {
        nextExpiration_ = next;

        TimeStamp microseconds = nextExpiration_ - getNowTimeStamp();

        if (microseconds < 100) {
            microseconds = 100;
        }

        timespec ts;
        ts.tv_sec = microseconds / kMicroSecondsPerSecond;
        ts.tv_nsec = (microseconds % kMicroSecondsPerSecond) * 1000;

        itimerspec newValue;
        bzero(&newValue, sizeof(newValue));

        newValue.it_value = ts;

        int ret = timerfd_settime(timerFd_, 0, &newValue, nullptr);

        if (ret) {
            std::cout << strerror(errno) << std::endl;

            std::cout << "timerfd_settime error" << std::endl;
        }
    }
}

void TimerQueue::insert(std::shared_ptr<Timer> timer) {
    TimeStamp when = timer->getExpiration();

    timers_.insert(Entry(when, std::move(timer)));
}

void TimerQueue::update(const std::vector<Entry>& entries, TimeStamp now) {
    for (const Entry& entry : entries) {
        if (entry.second->repeat()) {
            entry.second->restart(now);
            insert(entry.second);
        }
    }

    if (!timers_.empty()) {
        resetTimerFd();
    }
}

void TimerQueue::handleRead() {
    TimeStamp now = getNowTimeStamp();

    // 先读timerFd
    uint64_t readSize;
    read(timerFd_, &readSize, sizeof(readSize));
    assert(readSize == sizeof(readSize));

    auto entries = getExpired(now);

    for (const Entry& entry : entries) {
        entry.second->run();
    }

    nextExpiration_ = -1;
    update(entries, now);
}

}  // namespace SpongeBob