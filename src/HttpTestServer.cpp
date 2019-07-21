#include "HttpTestServer.h"

HttpTestServer::HttpTestServer(EventLoop* loop, int port, int threadNum)
    : tcpServer_(loop, port, threadNum) {
    tcpServer_.setMsgCallBack(std::bind(&HttpTestServer::handleMsg, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));
}

void HttpTestServer::start() { tcpServer_.start(); }

static std::string msg =
    "HTTP/1.1 200 OK\r\n"
    "Server: HttpTestServer/0.1\r\n"
    "Content-Type: text/html\r\n"
    "Connection: Keep-Alive\r\n"
    "Content-Length: 101\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<title>SpongeBob</title>"
    "</head>"
    "<body>"
    "<h1>HttpTestServer</h1>"
    "</body>"
    "</html>";

void HttpTestServer::handleMsg(const spTcpConnection& spConn, std::string& s) {
    std::string m;
    s.swap(m);
    spConn->send(msg);
}