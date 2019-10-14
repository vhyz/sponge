#pragma once

#include <sponge/TcpServer.h>
#include "HttpRequestParser.h"
#include "HttpResponse.h"

namespace sponge {

namespace http {

class HttpServer {
   public:
    using HttpCallBack = std::function<void(const HttpRequest&, HttpResponse*)>;

    HttpServer(EventLoop* loop, uint16_t port, int threadNum);

    ~HttpServer() = default;

    void setHttpCallBack(HttpCallBack cb) { httpCallBack_ = std::move(cb); }

    void start() { tcpServer_.start(); }

   private:
    void onConnection(const spTcpConnection& conn);

    void onMessage(const spTcpConnection& conn, ChannelBuffer& buffer);

   private:
    TcpServer tcpServer_;

    HttpCallBack httpCallBack_;
};
}  // namespace http
}  // namespace sponge