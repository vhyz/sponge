#ifndef SPONGEBOB_ECHOSERVER_H
#define SPONGEBOB_ECHOSERVER_H

#include "TcpServer.h"

class EchoServer {
   public:

    EchoServer(EventLoop* loop,int port,int threadNum);

    void start();

   private:
    void handleMsg(const spTcpConnection& spConn, std::string& s);

    TcpServer tcpServer_;
};

#endif
