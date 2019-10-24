#include "TcpClient.h"
#include <unistd.h>
#include "Logger.h"

namespace sponge {

TcpClient::TcpClient(EventLoop* loop, const InetAddress& peer, bool reconnect)
    : loop_(loop),
      peer_(peer),
      connector_(loop_, peer_),
      reconnect_(reconnect) {
    connector_.setNewConnectionCallBack(
        std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
}

void TcpClient::newConnection(int fd) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int err = getsockname(fd, (sockaddr*)&addr, &len);
    if (err < 0) {
        ERROR("TcpClient::newConnection getsockname");
        close(fd);
        return;
    }
    InetAddress local;
    local.setAddress(addr);
    auto spConn = std::make_shared<TcpConnection>(fd, loop_, local, peer_);
    spConn->setMessageCallBack(messageCallBack_);
    spConn->setConnCallBack(connectionCallBack_);
    spConn->setSendCallBack(writeCompleteCallBack_);
    spConn->setCloseCallBack(
        std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        conn_ = std::move(spConn);
    }

    loop_->runInLoop(std::bind(&TcpConnection::connEstablished, conn_));
}

void TcpClient::removeConnection(const TcpConnection::Ptr& spConn) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        conn_.reset();
    }

    if (reconnect_)
        connector_.start();
}

}  // namespace sponge