#include "HttpServer.h"
#include <sponge/Logger.h>

namespace sponge {
namespace http {

using namespace std::placeholders;

HttpServer::HttpServer(EventLoop* loop, uint16_t port, int threadNum)
    : tcpServer_(loop, port, threadNum) {
    tcpServer_.setConnCallBack(std::bind(&HttpServer::onConnection, this, _1));
    tcpServer_.setMessageCallBack(
        std::bind(&HttpServer::onMessage, this, _1, _2));
}

void HttpServer::onConnection(const TcpConnection::Ptr& conn) {
    INFO("connection %s -> %s %s", conn->getPeerAddr().getIpAndPort().c_str(),
         conn->getLocalAddr().getIpAndPort().c_str(),
         conn->isConnected() ? "up" : "down");
    if (conn->isConnected()) {
        conn->setContext(std::make_shared<HttpRequestParser>());
    }
}

void HttpServer::onMessage(const TcpConnection::Ptr& conn, ChannelBuffer& buffer) {
    HttpRequestParser* parser =
        std::any_cast<std::shared_ptr<HttpRequestParser>>(conn->getContext())
            .get();

    const HttpRequest& request = parser->getHttpRequest();
    int parseLength = parser->parse(buffer.readPtr(), buffer.readableBytes());

    std::unique_ptr<HttpResponse> response;
    if (parseLength < 0) {
        response = std::make_unique<HttpResponse>(HttpVersion11);
        response->setStatusCode(400);
        response->setKeepAlive(false);
    } else {
        buffer.readNBytes(static_cast<size_t>(parseLength));
        if (parser->isComplete()) {
            if (request.getHttpVersion() != HttpVersionOther) {
                response =
                    std::make_unique<HttpResponse>(request.getHttpVersion());
                response->setKeepAlive(request.isKeepAlive());
                httpCallBack_(request, response.get());
            } else {
                response = std::make_unique<HttpResponse>(HttpVersion11);
                response->setStatusCode(505);
                response->setKeepAlive(false);
            }
        }
    }

    if (response) {
        conn->send(response->serialize());
        if (!response->isKeepAlive()) {
            conn->shutdown();
        }

        parser->clear();
    }
}

}  // namespace http
}  // namespace sponge