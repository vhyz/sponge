#ifndef SPONGEBOB_TCPSERVER_H
#define SPONGEBOB_TCPSERVER_H

#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "TcpConnection.h"

class TcpServer {
   public:
    using CallBack = TcpConnection::CallBack;

    using MsgCallBack = TcpConnection::MessageCallBack;

    TcpServer(EventLoop* loop, int port, int threadNum);

    ~TcpServer();

    void setNewConnCallBack(CallBack cb) { newConnCallBack_ = std::move(cb); }

    void setMsgCallBack(MsgCallBack cb) { msgCallBack_ = std::move(cb); }

    void setWriteCompleteCallBack(CallBack cb) {
        writeCompleteCallBack_ = std::move(cb);
    }

    // 启动服务器
    void start();

   private:
    // 服务器FD
    int serverFd_;

    // 服务器事件
    Channel serverChannel;

    // 服务器fd所属的loop
    EventLoop* loop_;

    // 连接数量
    size_t connCount;

    // 连接表
    std::map<int, spTcpConnection> connMap_;

    // IO线程池
    EventLoopThreadPool threadPool_;

    // 保护map的锁
    std::mutex mutex_;

    // 回调函数
    CallBack newConnCallBack_;
    MsgCallBack msgCallBack_;
    CallBack writeCompleteCallBack_;

    void setNonblock(int fd);

    void setReuse(int fd);

    void listenPort(int port);

    void onNewConn();

    void removeConn(const spTcpConnection& spConn);

    static const size_t MAXCONN = 30000;
};

#endif