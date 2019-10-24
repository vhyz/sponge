#include "TcpServer.h"
#include <signal.h>
#include <strings.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>
#include "Logger.h"

namespace sponge {

TcpServer::TcpServer(EventLoop* loop, uint16_t port, int threadNum)
    : serverFd_(-1),
      serverEvent_(),
      loop_(loop),
      connCount(0),
      connMap_(),
      threadPool_(loop, threadNum) {
    signal(SIGPIPE, SIG_IGN);
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd_ < 0) {
        FATAL("socket error");
    }

    setNonblock(serverFd_);
    setReuse(serverFd_);

    // 监听端口
    listenPort(port);

    serverEvent_.setFd(serverFd_);
    serverEvent_.enableRead();
    serverEvent_.setEventCallBack([this](uint32_t events) {
        if (events & Event::SPEV_READ) {
            onNewConn();
        }
    });
}

TcpServer::~TcpServer() {
    loop_->removeEvent(&serverEvent_);
    close(serverFd_);
}

void TcpServer::start() {
    INFO("TcpServer port:%d, with %d IO threads start", localAddr_.getPort(),
         threadPool_.getNumThread());
    threadPool_.start();

    loop_->addEvent(&serverEvent_);
}

void TcpServer::setNonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        FATAL("setNonblock error");
    }
}

void TcpServer::setReuse(int fd) {
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void TcpServer::listenPort(uint16_t port) {
    sockaddr_in addr;

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    localAddr_.setAddress(addr);

    if (bind(serverFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        FATAL("bind error");
    }

    if (listen(serverFd_, SOMAXCONN) < 0) {
        FATAL("listen error");
    }
}

void TcpServer::onNewConn() {
    sockaddr_in clientAddr;
    int clientFd;
    socklen_t len = sizeof(clientAddr);
    while ((clientFd = accept(serverFd_, (sockaddr*)&clientAddr, &len)) > 0) {
        if (connCount + 1 >= MAXCONN) {
            close(clientFd);
            continue;
        }

        setNonblock(clientFd);

        EventLoop* ioLoop = threadPool_.getNextLoop();
        InetAddress peer;
        peer.setAddress(clientAddr);

        TcpConnection::Ptr spConn =
            std::make_shared<TcpConnection>(clientFd, ioLoop, localAddr_, peer);

        spConn->setMessageCallBack(messageCallBack_);
        spConn->setSendCallBack(writeCompleteCallBack_);
        spConn->setConnCallBack(connCallBack_);
        spConn->setCloseCallBack(
            std::bind(&TcpServer::removeConn, this, std::placeholders::_1));

        connMap_[clientFd] = spConn;
        connCount++;

        ioLoop->runInLoop(std::bind(&TcpConnection::connEstablished, spConn));
    }
}

void TcpServer::removeConn(const TcpConnection::Ptr& spConn) {
    loop_->runInLoop([this, spConn]() {
        size_t n = connMap_.erase(spConn->getFd());
        assert(n == 1);
        --connCount;
    });
}

}  // namespace sponge