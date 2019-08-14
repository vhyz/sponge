#include <iostream>
#include "../../SpongeBob/TcpClient.h"

int main() {
    EventLoop loop;
    InetAddress address("127.0.0.1", 5000);
    TcpClient tcpClient(&loop, address);
    tcpClient.setConnectionCallBack([](const spTcpConnection& spConn) {
        std::cout << spConn->getLocalAddr().getIpAndPort() << std::endl;
        std::cout << spConn->getPeerAddr().getIpAndPort() << std::endl;
        spConn->send("hello world");
    });
    tcpClient.setMessageCallBack(
        [](const spTcpConnection& spConn, std::string& msg) {
            std::cout << "收到消息" << std::endl;
            std::cout << msg << std::endl;
        });
    tcpClient.start();

    loop.loop();
}