#include "Connector.h"
#include <errno.h>
#include <sponge/Logger.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

namespace sponge {

int createSocket() {
    int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    return fd;
}

double Connector::retryInterval = 3;

Connector::Connector(EventLoop* loop, const InetAddress& peer)
    : loop_(loop), peer_(peer) {
    event_.setEventCallBack([this](uint32_t events) {
        INFO("handleWrite");
        if (events & Event::SPEV_WRITE) {
            handleWrite();
        }
    });
}

Connector::~Connector() { loop_->cancel(retryTimer_); }

void Connector::start() {
    if (loop_->isInLoopThread()) {
        startInLoop();
    } else {
        loop_->runInLoop(std::bind(&Connector::startInLoop, this));
    }
}

void Connector::startInLoop() {
    fd_ = createSocket();

    int err = connect(fd_, (sockaddr*)&peer_.getSockaddr(), sizeof(peer_));
    if (err) {
        event_.setFd(fd_);
        event_.enableWrite();

        loop_->addEvent(&event_);
    } else {
        newConnectionCallBack_(fd_);
    }
}

void Connector::handleWrite() {
    loop_->removeEvent(&event_);
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

}  // namespace sponge