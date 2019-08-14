#include "../../SpongeBob/TcpServer.h"

int main() {
    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 7);
    tcpServer.setMessageCallBack(
        [](const spTcpConnection& spConn, std::string& msg) {
            std::string s;
            s.swap(msg);
            spConn->send(s);
        });
    tcpServer.start();
    loop.loop();
}