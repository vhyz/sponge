#pragma once

#include <map>
#include "CallBack.h"
#include "Event.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "TcpConnection.h"
#include "noncopyable.h"

namespace sponge {

class TcpServer :public noncopyable {
   public:
    TcpServer(EventLoop* loop, uint16_t port, int threadNum);

    ~TcpServer();

    void setConnCallBack(ConnectionCallBack cb) {
        connCallBack_ = std::move(cb);
    }

    void setMessageCallBack(MessageCallBack cb) {
        messageCallBack_ = std::move(cb);
    }

    void setWriteCompleteCallBack(ConnectionCallBack cb) {
        writeCompleteCallBack_ = std::move(cb);
    }

    // 启动服务器
    void start();

   private:
    // 服务器FD
    int serverFd_;

    // 服务器事件
    Event serverEvent_;

    // 服务器fd所属的loop
    EventLoop* loop_;

    // 连接数量
    size_t connCount;

    // 连接表
    std::map<int, TcpConnection::Ptr> connMap_;

    // IO线程池
    EventLoopThreadPool threadPool_;

    // 保护map的锁
    std::mutex mutex_;

    // 回调函数
    ConnectionCallBack connCallBack_;
    MessageCallBack messageCallBack_;
    ConnectionCallBack writeCompleteCallBack_;

    InetAddress localAddr_;

    static const size_t MAXCONN = 30000;

    void setNonblock(int fd);

    void setReuse(int fd);

    void listenPort(uint16_t port);

    void onNewConn();

    void removeConn(const TcpConnection::Ptr& spConn);
};

}  // namespace sponge
