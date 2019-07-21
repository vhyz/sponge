#include "TcpConnection.h"
#include <unistd.h>
#include <iostream>

const int BufferSize = 4096;

TcpConnection::TcpConnection(int fd, EventLoop* loop, sockaddr_in clientAddr)
    : fd_(fd),
      loop_(loop),
      channel_(),
      clientAddr_(clientAddr),
      inputBuffer_(),
      outputBuffer_(),
      connected(true) {
    channel_.setFd(fd_);
    channel_.setEvents(EPOLLIN);
    channel_.setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
    channel_.setErrorCallBack(std::bind(&TcpConnection::handleError, this));
    channel_.setReadCallBack(std::bind(&TcpConnection::handleRead, this));
    channel_.setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
}

// 析构函数，不可能由主线程运行，因为主线程已经cleanup
TcpConnection::~TcpConnection() {
    loop_->deleteChannle(&channel_);
    close(fd_);
}

void TcpConnection::send(const std::string& msg) {
    // FIXME 存在线程安全问题
    // 若主线程连续调用此函数，outputBuffer_会出现安全问题
    // 解决方法为上锁或者写一个更好的Buffer
    outputBuffer_ += msg;
    if (loop_->runInLoop()) {
        sendInLoop();
    } else {
        loop_->addTask(
            std::bind(&TcpConnection::sendInLoop, shared_from_this()));
    }
}

void TcpConnection::sendInLoop() {
    if (!connected) {
        return;
    }

    int res = writeMsg();

    if (res > 0) {
        uint32_t events = channel_.getEvents();

        if (outputBuffer_.size() > 0) {
            // 缓冲区还有空间 未发送完毕
            // 监控可写事件
            channel_.setEvents(events | EPOLLOUT);
            loop_->updateChannel(&channel_);
        } else {
            // 取消可写事件
            channel_.setEvents(events & (~EPOLLOUT));
            if (sendCallBack_)
                sendCallBack_(shared_from_this());
            loop_->updateChannel(&channel_);
        }
    } else if (res < 0) {
        // 发生错误
        handleError();
    } else {
        handleClose();
    }
}

void TcpConnection::addChannelToLoop() {
    loop_->addTask(std::bind(&EventLoop::addChannel, loop_, &channel_));
}

void TcpConnection::handleRead() {
    int res = readMsg();
    std::cout << "TcpConnection::handleRead  " << res << "  bytes" <<
    std::endl;
    if (res > 0) {
        if (messageCallBack_)
            messageCallBack_(shared_from_this(), inputBuffer_);
    } else if (res == 0) {
        handleClose();
    } else {
        handleError();
    }
}

void TcpConnection::handleWrite() { sendInLoop(); }

void TcpConnection::handleError() {}

void TcpConnection::handleClose() {
    std::cout << "TcpConnection::handleClose" << std::endl;
    if (!connected) {
        return;
    }
    if (closeCallBack_)
        closeCallBack_(shared_from_this());
    connected = false;
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
            // 客户端关闭连接 但是还是等待RDHUP事件再关闭连接
            return res;
        }
    }
}

int TcpConnection::writeMsg() {
    int length = outputBuffer_.size();

    int nwrite = 0;

    int ret;

    while (true) {
        // 滑动指针的方式发送数据
        int writeByets =
            write(fd_, outputBuffer_.c_str() + nwrite, length - nwrite);

        if (writeByets > 0) {
            nwrite += writeByets;

            if (nwrite == length) {
                // 已写完
                ret = nwrite;
                break;
            }

        } else if (writeByets < 0) {
            if (errno == EAGAIN) {
                // 内核缓冲区已满
                ret = nwrite;
                break;
            } else if (errno == EINTR) {
                // 系统中断 再次尝试
                continue;
            } else if (errno == EPIPE) {
                // 对端已关闭连接
                ret = -1;
                break;
            } else {
                ret = -1;
                break;
            }
        } else {
            // 等于0 应该不会出现等于0的情况

            ret = 0;
        }
    }

    outputBuffer_.erase(outputBuffer_.begin(), outputBuffer_.begin() + nwrite);

    return ret;
}