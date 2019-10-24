#pragma once

#include <mutex>
#include "CallBack.h"
#include "Connector.h"
#include "EventLoop.h"
#include "TcpConnection.h"

namespace sponge {

class TcpClient {
   public:
    TcpClient(EventLoop* loop, const InetAddress& peer, bool reconnect = false);

    ~TcpClient() = default;

    void connect() { connector_.start(); }

    void disconnect() {
        if (conn_) {
            conn_->shutdown();
        }
    }

    void setMessageCallBack(MessageCallBack cb) {
        messageCallBack_ = std::move(cb);
    }

    void setConnectionCallBack(ConnectionCallBack cb) {
        connectionCallBack_ = std::move(cb);
    }

    void setWriteCompleteCallBack(ConnectionCallBack cb) {
        writeCompleteCallBack_ = std::move(cb);
    }

   private:
    void newConnection(int fd);

    void removeConnection(const TcpConnection::Ptr& spConn);

   private:
    EventLoop* loop_;

    const InetAddress peer_;

    Connector connector_;

    bool reconnect_;

    std::mutex mutex_;

    TcpConnection::Ptr conn_;

    MessageCallBack messageCallBack_;
    ConnectionCallBack connectionCallBack_;
    ConnectionCallBack writeCompleteCallBack_;
};

}  // namespace sponge
