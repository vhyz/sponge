#include <sponge/EventLoopThread.h>
#include <sponge/Logger.h>
#include <sponge/TcpClient.h>
#include <iostream>
#include <string>

using namespace std::placeholders;
using namespace sponge;

class EchoClient {
   public:
    EchoClient(EventLoop* loop, const InetAddress& serverAddress)
        : client_(loop, serverAddress) {
        client_.setConnectionCallBack(
            std::bind(&EchoClient::onConnection, this, _1));
        client_.setMessageCallBack(
            std::bind(&EchoClient::onMessage, this, _1, _2));
    }

    void onConnection(const spTcpConnection& spConn) {
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

    void onMessage(const spTcpConnection spConn, ChannelBuffer& buffer) {
        std::cout << "recv msg: " << buffer.readAllBytesAsString() << std::endl;
    }

    void send(const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (conn_) {
            conn_->send(msg);
        }
    }

    void connect() { client_.connect(); }

    void disconnect() { client_.disconnect(); }

   private:
    TcpClient client_;

    spTcpConnection conn_;

    std::mutex mutex_;
};

int main(int argc, char** argv) {
    if (argc == 3) {
        EventLoopThread loopThread;
        loopThread.start();

        uint16_t port = atoi(argv[2]);
        InetAddress serverAddress(argv[1], port);

        EchoClient client(loopThread.getEventLoop(), serverAddress);
        client.connect();

        std::string line;
        while (getline(std::cin, line)) {
            client.send(line);
        }
    } else {
        printf("Usage: %s ip port\n", argv[0]);
    }
}