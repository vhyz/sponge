#include "TcpServer.h"
#include <signal.h>
#include <strings.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>

TcpServer::TcpServer(EventLoop* loop, int port, int threadNum)
    : serverFd_(-1),
      serverChannel(),
      loop_(loop),
      connCount(0),
      connMap_(),
      threadPool_(loop, threadNum) {
    signal(SIGPIPE, SIG_IGN);
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd_ < 0) {
        std::cout << "socket error" << std::endl;
        exit(1);
    }

    setNonblock(serverFd_);
    setReuse(serverFd_);

    // 监听端口
    listenPort(port);

    serverChannel.setFd(serverFd_);
    serverChannel.setEvents(EPOLLIN);
    serverChannel.setReadCallBack(std::bind(&TcpServer::onNewConn, this));
}

TcpServer::~TcpServer() {
    loop_->deleteChannle(&serverChannel);
    close(serverFd_);
}

void TcpServer::start() {
    std::cout << "Tcp Server Start" << std::endl;

    threadPool_.start();

    loop_->addChannel(&serverChannel);
}

void TcpServer::setNonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        std::cout << "setNonblock error" << std::endl;
        exit(1);
    }
}

void TcpServer::setReuse(int fd) {
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void TcpServer::listenPort(int port) {
    sockaddr_in addr;

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cout << "bind error" << std::endl;
        exit(1);
    }

    if (listen(serverFd_, SOMAXCONN) < 0) {
        std::cout << "listen error" << std::endl;
        exit(1);
    }
}

void TcpServer::onNewConn() {
    sockaddr_in clientAddr;
    int clientFd;
    socklen_t len = sizeof(clientAddr);
    while ((clientFd = accept(serverFd_, (sockaddr*)&clientAddr, &len)) > 0) {
        // std::cout << "New client from IP:" << inet_ntoa(clientAddr.sin_addr)
        //          << ":" << ntohs(clientAddr.sin_port) << std::endl;
        if (connCount + 1 >= MAXCONN) {
            close(clientFd);
        }

        setNonblock(clientFd);

        EventLoop* ioLoop = threadPool_.getNextLoop();
        spTcpConnection spConn =
            std::make_shared<TcpConnection>(clientFd, ioLoop, clientAddr);

        spConn->setMessageCallBack(msgCallBack_);
        spConn->setSendCallBack(writeCompleteCallBack_);
        spConn->setConnCallBack(connCallBack_);
        spConn->setCloseCallBack(
            std::bind(&TcpServer::removeConn, this, std::placeholders::_1));
 
        {
            std::lock_guard<std::mutex> lock(mutex_);
            connMap_[clientFd] = spConn;
            connCount++;
        }
        ioLoop->addTask(std::bind(&TcpConnection::connEstablished, spConn));
    }
}

void TcpServer::removeConn(const spTcpConnection& spConn) {
    const sockaddr_in& clientAddr = spConn->getSockAddr();
    // std::cout << "Remove client from IP:" << inet_ntoa(clientAddr.sin_addr)
    //         << ":" << ntohs(clientAddr.sin_port) << "  FD:" <<
    //         spConn->getFd()
    //          << std::endl;

    std::lock_guard<std::mutex> lock(mutex_);
    connMap_.erase(spConn->getFd());
    --connCount;
}