#include "Connector.h"
#include <SpongeBob/Logger.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int createSocket() {
    int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    return fd;
}

double Connector::retryInterval = 3;

Connector::Connector(EventLoop* loop, const InetAddress& peer)
    : loop_(loop), peer_(peer) {
    channel_.setWriteCallBack(std::bind(&Connector::handleWrite, this));
}

Connector::~Connector() { loop_->cancel(retryTimer_); }

void Connector::start() {
    fd_ = createSocket();

    int err = connect(fd_, (sockaddr*)&peer_.getSockaddr(), sizeof(peer_));
    if (err) {
        channel_.setFd(fd_);
        channel_.setEvents(EPOLLOUT);

        loop_->addChannel(&channel_);
    } else {
        newConnectionCallBack_(fd_);
    }
}

void Connector::handleWrite() {
    loop_->deleteChannle(&channel_);
    int err;
    socklen_t len = sizeof(err);
    int ret = getsockopt(fd_, SOL_SOCKET, SO_ERROR, &err, &len);
    if (ret) {
        ERROR("Connector::handleWrite() getsockopt");
        return;
    }
    errno = err;

    if (err) {
        ERROR(strerror(errno));
        if (errorCallBack_) {
            errorCallBack_();
        }
        close(fd_);
        retry();
    } else {
        retryTimer_.reset();
        if (newConnectionCallBack_) {
            newConnectionCallBack_(fd_);
        }
    }
}

void Connector::retry() {
    retryTimer_ =
        loop_->runAfter(retryInterval, std::bind(&Connector::start, this));
}