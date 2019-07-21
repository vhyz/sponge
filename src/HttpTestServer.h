#ifndef SPONGEBOB_HTTPTESTSEVER_H
#define SPONGEBOB_HTTPTESTSEVER_H


#include "TcpServer.h"

class HttpTestServer {
   public:
    HttpTestServer(EventLoop* loop, int port, int threadNum);

    void start();

   private:
    void handleMsg(const spTcpConnection& spConn, std::string& s);

    TcpServer tcpServer_;
};

#endif