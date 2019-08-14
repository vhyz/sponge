#include "TcpClient.h"
#include <unistd.h>
#include <iostream>

TcpClient::TcpClient(EventLoop* loop, const InetAddress& peer)
    : loop_(loop), peer_(peer), connector_(loop_, peer_) {
    connector_.setNewConnectionCallBack(
        std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
}

void TcpClient::newConnection(int fd) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int err = getsockname(fd, (sockaddr*)&addr, &len);
    if (err < 0) {
        std::cout << "TcpClient::newConnection getsockname" << std::endl;
        close(fd);
        return;
    }
    InetAddress local;
    local.setAddress(addr);
    conn_ = std::make_shared<TcpConnection>(fd, loop_, local, peer_);
    conn_->setMessageCallBack(messageCallBack_);
    conn_->setConnCallBack(connectionCallBack_);
    conn_->setSendCallBack(writeCompleteCallBack_);
    loop_->runInLoop(std::bind(&TcpConnection::connEstablished, conn_));
}