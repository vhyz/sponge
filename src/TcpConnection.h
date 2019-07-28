#ifndef SPONGEBOB_TCPCONNECTION_H
#define SPONGEBOB_TCPCONNECTION_H

#include <arpa/inet.h>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include "Channel.h"
#include "EventLoop.h"

class TcpConnection;

using spTcpConnection = std::shared_ptr<TcpConnection>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
   public:
    using MessageCallBack =
        std::function<void(const spTcpConnection&, std::string&)>;

    using CallBack = std::function<void(const spTcpConnection&)>;

    TcpConnection(int fd, EventLoop* loop, sockaddr_in clientAddr);

    ~TcpConnection();

    // 主动发送消息，由其他线程调用时会将任务添加至loop中
    void send(const std::string& msg);

    // 在loop线程中发送消息
    void sendInLoop();

    // 一切回调函数设置好后，将channel添加至loop中
    void addChannelToLoop();

    // 各种回调函数，传递给连接的channel
    void handleRead();

    void handleWrite();

    void handleClose();

    void handleError();

    // 强制关闭
    void forceClose();

    int getFd() const { return fd_; }

    EventLoop* getLoop() const { return loop_; }

    const sockaddr_in& getSockAddr() const { return clientAddr_; }

    void setSendCallBack(CallBack cb) { sendCallBack_ = std::move(cb); }

    void setCloseCallBack(CallBack cb) { closeCallBack_ = std::move(cb); }

    void setErrorCallBack(CallBack cb) { errorCallBack_ = std::move(cb); }

    void setMessageCallBack(MessageCallBack cb) {
        messageCallBack_ = std::move(cb);
    }

   private:
    // socket fd;
    int fd_;

    // 连接所在的loop
    EventLoop* loop_;

    // 当前连接的事件
    Channel channel_;

    // 客户端的addr结构
    sockaddr_in clientAddr_;

    // 输入缓冲
    std::string inputBuffer_;

    // 输出缓冲
    std::string outputBuffer_;

    // 是否连接的标志位
    bool connected;

    CallBack sendCallBack_;
    CallBack closeCallBack_;
    CallBack errorCallBack_;
    MessageCallBack messageCallBack_;

    int readMsg();

    int writeMsg();
};

#endif