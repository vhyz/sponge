#include <sponge/EventLoop.h>
#include <sponge/Logger.h>
#include <sponge/TcpClient.h>
#include <iostream>
#include <mutex>
#include "codec.h"

using namespace std::placeholders;
using namespace sponge;

class ChatClient {
   public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddress,
               bool reconnect = false)
        : client_(loop, serverAddress, reconnect),
          codec_(std::bind(&ChatClient::onMessage, this, _1, _2)) {
        client_.setConnectionCallBack(
            std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallBack(
            std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2));
    }

    void onConnection(const TcpConnection::Ptr& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");

        std::lock_guard<std::mutex> lock(mutex_);
        if (spConn->isConnected()) {
            conn_ = spConn;
        } else {
            conn_.reset();
        }
    }

    void onMessage(const TcpConnection::Ptr& spConn, const std::string& msg) {
        std::cout << "recv message: " << msg << std::endl;
    }

    void send(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (conn_) {
            codec_.send(conn_, msg);
        }
    }

    void connect() { client_.connect(); }

    void disconnect() { client_.disconnect(); }

   private:
    TcpClient client_;

    LengthHeaderCodec codec_;

    TcpConnection::Ptr conn_;

    std::mutex mutex_;
};

int main(int argc, char** argv) {
    if (argc == 3) {
        EventLoopThread loopThread;
        loopThread.start();

        uint16_t port = atoi(argv[2]);
        InetAddress serverAddress(argv[1], port);
        std::cout << argv[1] << std::endl;
        ChatClient client(loopThread.getEventLoop(), serverAddress);
        std::cout << argv[1] << std::endl;
        client.connect();
        std::string line;
        while (getline(std::cin, line)) {
            client.send(line);
        }
    } else {
        printf("Usage: %s ip port", argv[0]);
    }
}