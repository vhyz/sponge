#include <sponge/Logger.h>
#include <sponge/TcpServer.h>
#include <stdio.h>
#include <mutex>
#include <set>
#include "codec.h"

using namespace std::placeholders;
using namespace sponge;

class ChatServer {
   public:
    ChatServer(EventLoop* loop, uint16_t port, int numThreads)
        : tcpServer_(loop, port, numThreads),
          codec_(std::bind(&ChatServer::onMessage, this, _1, _2)) {
        tcpServer_.setMessageCallBack(
            std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2));
        tcpServer_.setConnCallBack(
            std::bind(&ChatServer::onConnection, this, _1));
    }

    void onConnection(const spTcpConnection& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");

        std::lock_guard<std::mutex> lock(mutex_);

        if (spConn->isConnected()) {
            connectionList_.insert(spConn);
        } else {
            connectionList_.erase(spConn);
        }
    }

    void onMessage(const spTcpConnection& spConn, const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const spTcpConnection& spConn : connectionList_) {
            codec_.send(spConn, msg);
        }
    }

    void start() { tcpServer_.start(); }

   private:
    TcpServer tcpServer_;

    std::mutex mutex_;

    std::set<spTcpConnection> connectionList_;

    LengthHeaderCodec codec_;
};

int main(int argc, char** argv) {
    if (argc > 1) {
        EventLoop loop;
        uint16_t port = atoi(argv[1]);
        int numThraeds = 7;
        if (argc > 2) {
            numThraeds = atoi(argv[2]);
        }
        ChatServer server(&loop, port, numThraeds);
        server.start();
        loop.loop();
    } else {
        printf("Usage: %s port [numThreads]\n", argv[0]);
    }
}