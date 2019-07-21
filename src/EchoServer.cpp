#include "EchoServer.h"
#include <iostream>

EchoServer::EchoServer(EventLoop* loop, int port, int threadNum)
    : tcpServer_(loop, port, threadNum) {
    tcpServer_.setMsgCallBack(std::bind(&EchoServer::handleMsg, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
}

void EchoServer::start() { tcpServer_.start(); }

void EchoServer::handleMsg(const spTcpConnection& spConn, std::string& s) {
    std::string msg;
    msg.swap(s);
    spConn->send(msg);
}