#pragma once

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <any>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include "CallBack.h"
#include "ChannelBuffer.h"
#include "Event.h"
#include "EventLoop.h"
#include "InetAddress.h"

namespace sponge {

class TcpConnection;

using spTcpConnection = std::shared_ptr<TcpConnection>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
   public:
    TcpConnection(int fd, EventLoop* loop, const InetAddress& localAddr,
                  const InetAddress& peerAddr,
                  size_t readHighWaterMark = 1024 * 1024 * 4);

    ~TcpConnection();

    // 主动发送消息，由其他线程调用时会将任务添加至loop中
    void send(std::string_view msg);

    void send(const void* buf, size_t len);

    // 各种回调函数，传递给连接的channel
    void handleRead();

    void handleWrite();

    void handleClose();

    // 强制关闭
    void forceClose();

    void setReadHighWaterMark(size_t readHighWaterMark) {
        readHighWaterMark_ = readHighWaterMark;
    }

    int getFd() const { return fd_; }

    EventLoop* getLoop() const { return loop_; }

    const InetAddress& getLocalAddr() const { return localAddr_; }

    const InetAddress& getPeerAddr() const { return peerAddr_; }

    void setSendCallBack(ConnectionCallBack cb) {
        sendCallBack_ = std::move(cb);
    }

    void setCloseCallBack(ConnectionCallBack cb) {
        closeCallBack_ = std::move(cb);
    }

    void setErrorCallBack(ConnectionCallBack cb) {
        errorCallBack_ = std::move(cb);
    }

    void setMessageCallBack(MessageCallBack cb) {
        messageCallBack_ = std::move(cb);
    }

    void setConnCallBack(ConnectionCallBack cb) {
        connCallBack_ = std::move(cb);
    }

    void setReadHighWaterCallBack(ConnectionCallBack cb) {
        readHighWaterCallBack_ = std::move(cb);
    }

    void connEstablished();

    void shutdown();

    void shutdownInLoop();

    void setContext(const std::any& context) { context_ = context; }

    void setContext(std::any&& context) { context_ = std::move(context_); }

    const std::any& getContext() const { return context_; }

    std::any& getContext() { return context_; }

    bool isConnected() const { return connected; }

    void setTcpNoDelay(bool type) {
        int on = type ? 1 : 0;
        setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &on, sizeof(on));
    }

   private:
    // 在loop线程中发送消息
    void sendInLoop(std::string_view msg);

    void sendInLoop(const void* buf, size_t len);

    // socket fd;
    int fd_;

    // 连接所在的loop
    EventLoop* loop_;

    // 当前连接的事件
    Event event_;

    // 本地与服务器的addr
    InetAddress localAddr_;
    InetAddress peerAddr_;

    // 输入缓冲区与输出缓冲区
    ChannelBuffer inputBuffer_;
    ChannelBuffer outputBuffer_;

    // 是否连接的标志位
    bool connected;

    // 半关闭标志位
    bool half_close_;

    size_t readHighWaterMark_;

    ConnectionCallBack sendCallBack_;
    ConnectionCallBack closeCallBack_;
    ConnectionCallBack errorCallBack_;
    MessageCallBack messageCallBack_;
    ConnectionCallBack connCallBack_;
    ConnectionCallBack readHighWaterCallBack_;

    std::any context_;

    int readMsg();

    int writeMsg();
};

}  // namespace sponge
