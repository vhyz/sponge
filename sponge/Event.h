#pragma once

#include <functional>
#include <memory>
#include "Logger.h"

namespace sponge {

class Event {
   public:
    const static uint32_t SPEV_NONE = 0x000;
    const static uint32_t SPEV_READ = 0x001;
    const static uint32_t SPEV_WRITE = 0x002;
    const static uint32_t SPEV_TIMEOUT = 0x004;

    Event() : events_(SPEV_NONE) {}

    Event(int fd, uint32_t events = SPEV_NONE) : fd_(fd), events_(events) {}

    ~Event() {}

    using EventCallBack = std::function<void(uint32_t)>;

    void enableRead() { events_ |= SPEV_READ; }

    void enableWrite() { events_ |= SPEV_WRITE; }

    void disableRead() { events_ &= ~SPEV_READ; }

    void disableWrite() { events_ &= ~SPEV_WRITE; }

    void setEvents(uint32_t events) { events_ = events; }

    uint32_t getEvents() const { return events_; }

    void setActiveEvents(uint32_t e) { activeEvents_ = e; }

    uint32_t getActiveEvents() const { return activeEvents_; }

    void setFd(uint32_t fd) { fd_ = fd; }

    int getFd() const { return fd_; }

    void setEventCallBack(EventCallBack cb) { eventCallBack_ = std::move(cb); }

    void handleEvent() {
        if (eventCallBack_) {
            eventCallBack_(activeEvents_);
        }
    }

   private:
    int fd_;

    uint32_t events_;

    uint32_t activeEvents_;

    EventCallBack eventCallBack_;
};

};  // namespace sponge