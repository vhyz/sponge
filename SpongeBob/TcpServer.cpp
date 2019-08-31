#include "TcpServer.h"
#include <signal.h>
#include <strings.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "Logger.h"

namespace SpongeBob {

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
        FATAL("socket error");
    }

    setNonblock(serverFd_);
    setReuse(serverFd_);

    // 监听端口
    listenPort(port);

    serverChannel.setFd(serverFd_);
    serverChannel.enableRead();
    serverChannel.setReadCallBack(std::bind(&TcpServer::onNewConn, this));
}

TcpServer::~TcpServer() {
    loop_->deleteChannle(&serverChannel);
    close(serverFd_);
}

void TcpServer::start() {
    INFO("TcpServer port:%d, with %d IO threads start", localAddr_.getPort(),
         threadPool_.getNumThread());

    threadPool_.start();

    loop_->addChannel(&serverChannel);
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

void TcpServer::listenPort(int port) {
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

        spTcpConnection spConn =
            std::make_shared<TcpConnection>(clientFd, ioLoop, localAddr_, peer);
        spConn->setTcpNoDelay();

        spConn->setMessageCallBack(messageCallBack_);
        spConn->setSendCallBack(writeCompleteCallBack_);
        spConn->setConnCallBack(connCallBack_);
        spConn->setCloseCallBack(
            std::bind(&TcpServer::removeConn, this, std::placeholders::_1));

        {
            std::lock_guard<std::mutex> lock(mutex_);
            connMap_[clientFd] = spConn;
            connCount++;
        }
        ioLoop->runInLoop(std::bind(&TcpConnection::connEstablished, spConn));
    }
}

void TcpServer::removeConn(const spTcpConnection& spConn) {
    std::lock_guard<std::mutex> lock(mutex_);
    connMap_.erase(spConn->getFd());
    --connCount;
}

}  // namespace SpongeBob