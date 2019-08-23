#include "TcpConnection.h"
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <string_view>
#include "Logger.h"

const int BufferSize = 65536;

TcpConnection::TcpConnection(int fd, EventLoop* loop,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : fd_(fd),
      loop_(loop),
      channel_(),
      localAddr_(localAddr),
      peerAddr_(peerAddr),
      inputBuffer_(),
      outputBuffer_(),
      connected(true),
      half_close_(false) {
    channel_.setFd(fd_);
    channel_.setEvents(EPOLLIN);
    channel_.setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
    channel_.setErrorCallBack(std::bind(&TcpConnection::handleError, this));
    channel_.setReadCallBack(std::bind(&TcpConnection::handleRead, this));
    channel_.setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
}

// 析构函数，不可能由主线程运行，因为主线程已经cleanup
TcpConnection::~TcpConnection() { close(fd_); }

void TcpConnection::send(std::string_view msg) {
    if (loop_->isInLoopThread()) {
        sendInLoop(msg);
    } else {
        INFO("asyio");
        INFO(std::string(msg).c_str());
        void (TcpConnection::*fp)(std::string_view msg) =
            &TcpConnection::sendInLoop;
        loop_->runInLoop(std::bind(fp, shared_from_this(), std::string(msg)));
    }
}

void TcpConnection::send(const void* buf, size_t len) {
    send(std::string_view(static_cast<const char*>(buf), len));
}

void TcpConnection::sendInLoop(std::string_view msg) {
    sendInLoop(msg.data(), msg.size());
}

void TcpConnection::sendInLoop(const void* buf, size_t len) {
    if (!connected) {
        WARN("connection disconnected");
        return;
    }
    ssize_t nwrite = 0;
    size_t remaining = len;
    bool faultError = false;
    if (outputBuffer_.empty()) {
        // 只尝试写一次
        nwrite = ::write(fd_, buf, len);

        if (nwrite >= 0) {
            INFO("sendInLoop %d bytes", len);
            remaining = len - nwrite;
            if (remaining == 0 && sendCallBack_) {
                loop_->queueInLoop(
                    std::bind(sendCallBack_, shared_from_this()));
            }
        } else {
            nwrite = 0;
            if (errno != EAGAIN) {
                if (errno == EPIPE || errno == ECONNRESET) {
                    faultError = true;
                }
            }
        }
    }

    if (!faultError && remaining > 0) {
        std::cout << remaining << std::endl;
        outputBuffer_.append(static_cast<const char*>(buf + nwrite), remaining);

        uint32_t events = channel_.getEvents();
        channel_.setEvents(events | EPOLLOUT);
        loop_->updateChannel(&channel_);
    }
}

void TcpConnection::shutdown() {
    loop_->runInLoop(
        std::bind(&TcpConnection::shutdownInLoop, shared_from_this()));
}

void TcpConnection::shutdownInLoop() {
    if (!connected) {
        return;
    }

    if (outputBuffer_.size() > 0) {
        half_close_ = true;
    } else {
        handleClose();
    }
}

void TcpConnection::handleRead() {
    int res = readMsg();
    // std::cout << "TcpConnection::handleRead  " << res << "  bytes"
    //          << " " << std::this_thread::get_id() << std::endl;
    if (res > 0) {
        if (messageCallBack_)
            messageCallBack_(shared_from_this(), inputBuffer_);
    } else if (res == 0) {
        handleClose();
    } else {
        handleError();
    }
}

void TcpConnection::handleWrite() {
    ssize_t nwrite = write(fd_, outputBuffer_.data(), outputBuffer_.size());
    if (nwrite > 0) {
        outputBuffer_.erase(outputBuffer_.begin(),
                            outputBuffer_.begin() + nwrite);
        if (outputBuffer_.empty()) {
            if (sendCallBack_) {
                loop_->queueInLoop(
                    std::bind(sendCallBack_, shared_from_this()));
            }

            uint32_t events = channel_.getEvents();
            channel_.setEvents(events & ~EPOLLOUT);
            loop_->updateChannel(&channel_);

            if (half_close_) {
                shutdownInLoop();
            }
        }
    } else {
        // FIXME 日志显示错误
    }
}

void TcpConnection::handleError() {
    // FIXME 日志记录错误
    // 一般不会调用这个函数
}

void TcpConnection::handleClose() {
    if (!connected) {
        return;
    }
    connected = false;

    loop_->deleteChannle(&channel_);
    auto guardThis = shared_from_this();
    if (connCallBack_)
        connCallBack_(guardThis);
    if (closeCallBack_)
        closeCallBack_(guardThis);
}

void TcpConnection::forceClose() {
    loop_->queueInLoop(
        std::bind(&TcpConnection::handleClose, shared_from_this()));
}

void TcpConnection::connEstablished() {
    loop_->addChannel(&channel_);
    if (connCallBack_)
        connCallBack_(shared_from_this());
}

int TcpConnection::readMsg() {
    char buf[BufferSize];

    int res = 0;

    while (true) {
        int readBytes = read(fd_, buf, sizeof(buf));

        if (readBytes > 0) {
            inputBuffer_.append(buf, readBytes);

            res += readBytes;
            // 若小于缓冲区大小，不必进行下一次read
            if (readBytes < BufferSize) {
                return res;
            }

        } else if (readBytes < 0) {
            if (errno == EINTR) {
                // 系统中断 再尝试read
                continue;

            } else if (errno == EAGAIN) {
                // 内核缓冲区数据已读完
                return res;

            } else {
                // 其他错误，返回-1即可
                return -1;
            }
        } else {
            // 客户端关闭连接 但是还是等待检测到关闭事件再关闭连接
            return res;
        }
    }
}