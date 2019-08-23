#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpClient.h>
#include <iostream>
#include <string>

class EchoClient {
   public:
   private:
    void onRead(const spTcpConnection& spConn, std::string& str) {
        std::cout << str << std::endl;
    }

    void onConnection(const spTcpConnection& spConn) {
        
    }

    std::mutex mutex_;
};

int main() {
    setLogLevel(LOG_LEVEL_DEBUG);
    EventLoop loop;
    InetAddress address("127.0.0.1", 5000);
    TcpClient tcpClient(&loop, address);
    tcpClient.setConnectionCallBack([](const spTcpConnection& spConn) {
        if (spConn->isConnected()) {
            std::cout << spConn->getLocalAddr().getIpAndPort() << std::endl;
            std::cout << spConn->getPeerAddr().getIpAndPort() << std::endl;
            std::cout << spConn->getFd() << std::endl;
            spConn->send("hello world");
        }
    });
    tcpClient.setMessageCallBack(
        [](const spTcpConnection& spConn, std::string& msg) {
            std::cout << "收到消息" << std::endl;
            std::cout << msg << std::endl;
        });
    tcpClient.start();

    loop.loop();
}