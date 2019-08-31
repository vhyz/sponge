#include "TcpClient.h"
#include <unistd.h>
#include "Logger.h"

namespace SpongeBob {

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
    conn_ = std::make_shared<TcpConnection>(fd, loop_, local, peer_);
    conn_->setMessageCallBack(messageCallBack_);
    conn_->setConnCallBack(connectionCallBack_);
    conn_->setSendCallBack(writeCompleteCallBack_);
    conn_->setCloseCallBack(
        std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));

    loop_->runInLoop(std::bind(&TcpConnection::connEstablished, conn_));
}

void TcpClient::removeConnection(const spTcpConnection& spConn) {
    conn_.reset();
    if (reconnect_)
        connector_.start();
}

}  // namespace SpongeBob