#pragma once

#include "Event.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Timer.h"
#include "noncopyable.h"

namespace sponge {

class Connector : public noncopyable {
   public:
    Connector(EventLoop* loop, const InetAddress& peer);

    ~Connector();

    void start();

    using NewConnectionCallBack = std::function<void(int)>;

    void setNewConnectionCallBack(NewConnectionCallBack cb) {
        newConnectionCallBack_ = std::move(cb);
    }

    void setErrorCallBack(CallBack cb) { errorCallBack_ = std::move(cb); }

   private:
    void handleWrite();

    void retry();

    void startInLoop();

    EventLoop* loop_;

    Event event_;

    int fd_;

    const InetAddress& peer_;

    NewConnectionCallBack newConnectionCallBack_;

    CallBack errorCallBack_;

    TimerId retryTimer_;

    static double retryInterval;
};

}  // namespace sponge
