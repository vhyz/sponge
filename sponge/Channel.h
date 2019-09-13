#pragma once

#include <sys/epoll.h>
#include <functional>

namespace sponge {

class Channel {
   public:
    Channel() = default;
    ~Channel() = default;

    using EventCallBack = std::function<void()>;

    void handleEvent();

    int getFd() const { return fd_; }

    void setFd(int fd) { fd_ = fd; }

    uint32_t getEvents() const { return events_; }

    void setEvents(int events) { events_ = events; }

    void setRevents(int revents) { revents_ = revents; }

    void enableRead() { events_ |= EPOLLIN; }

    void enableWrite() { events_ |= EPOLLOUT; }

    void disableRead() { events_ &= ~EPOLLIN; }

    void disableWrite() { events_ &= ~EPOLLOUT; }

    void setReadCallBack(EventCallBack cb) { readCallBack_ = std::move(cb); }

    void setWriteCallBack(EventCallBack cb) { writeCallBack_ = std::move(cb); }

    void setErrorCallBack(EventCallBack cb) { errorCallBack_ = std::move(cb); }

    void setCloseCallBack(EventCallBack cb) { closeCallBack_ = std::move(cb); }

   private:
    int fd_;

    uint32_t events_;

    uint32_t revents_;

    EventCallBack readCallBack_;
    EventCallBack writeCallBack_;
    EventCallBack errorCallBack_;
    EventCallBack closeCallBack_;
};

}  // namespace sponge

