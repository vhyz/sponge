#ifndef SPONGEBOB_CONNECTOR_H
#define SPONGEBOB_CONNECTOR_H

#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Timer.h"

namespace SpongeBob {

class Connector {
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

    Channel channel_;

    int fd_;

    const InetAddress& peer_;

    NewConnectionCallBack newConnectionCallBack_;

    CallBack errorCallBack_;

    TimerId retryTimer_;

    static double retryInterval;
};

}  // namespace SpongeBob

#endif