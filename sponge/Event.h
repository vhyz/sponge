#pragma once

#include <functional>
#include <memory>
#include "Logger.h"

namespace sponge {

class Event {
   public:
    const static uint32_t SPEV_READ;
    const static uint32_t SPEV_WRITE;
    const static uint32_t SPEV_NONE;

    Event() : events_(SPEV_NONE) {}

    Event(int fd, uint32_t events = SPEV_NONE) : fd_(fd), events_(events) {}

    ~Event() {}

    using EventCallBack = std::function<void(uint32_t)>;

    void enableRead() { events_ |= SPEV_READ; }

    void enableWrite() { events_ |= SPEV_WRITE; }

    void disableRead() { events_ &= ~SPEV_READ; }

    void disableWrite() { events_ &= ~SPEV_WRITE; }

    void setEvents(uint32_t events) { events_ = events; }

    void setFd(uint32_t fd) { fd_ = fd; }

    uint32_t getEvents() const { return events_; }

    int getFd() const { return fd_; }

    void setEventCallBack(EventCallBack cb) { eventCallBack_ = std::move(cb); }

    void handleEvent(uint32_t events) {
        if (eventCallBack_) {
            eventCallBack_(events);
        }
    }

   private:
    int fd_;

    uint32_t events_;

    EventCallBack eventCallBack_;
};

};  // namespace sponge