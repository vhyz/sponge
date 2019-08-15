#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpServer.h>

int main() {
    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 7);
    tcpServer.setConnCallBack([](const spTcpConnection& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");
    });
    tcpServer.setMessageCallBack(
        [](const spTcpConnection& spConn, std::string& msg) {
            std::string s;
            s.swap(msg);
            spConn->send(s);
        });
    tcpServer.start();
    loop.loop();
}