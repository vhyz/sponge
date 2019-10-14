#include "HttpServer.h"

namespace sponge {
namespace http {

using namespace std::placeholders;

HttpServer::HttpServer(EventLoop* loop, uint16_t port, int threadNum)
    : tcpServer_(loop, port, threadNum) {
    tcpServer_.setConnCallBack(std::bind(&HttpServer::onConnection, this, _1));
    tcpServer_.setMessageCallBack(
        std::bind(&HttpServer::onMessage, this, _1, _2));
}

void HttpServer::onConnection(const spTcpConnection& conn) {
    if (conn->isConnected()) {
        conn->setContext(std::make_any<HttpRequestParser>());
    }
}

void HttpServer::onMessage(const spTcpConnection& conn, ChannelBuffer& buffer) {
    HttpRequestParser* parser =
        std::any_cast<HttpRequestParser>(&conn->getContext());

    int parseLength = parser->parse(buffer.readPtr(), buffer.readableBytes());

    if (parseLength < 0) {
        return;
    }

    buffer.readNBytes(parseLength);
    if (parser->isComplete()) {
        HttpResponse response;
        httpCallBack_(parser->getHttpRequest(), &response);
        
    }
}

}  // namespace http
}  // namespace sponge