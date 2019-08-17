#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpServer.h>

int main() {
    setLogLevel(LOG_LEVEL_DEBUG);
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
            INFO("handleRead %d bytes", msg.size());
            std::string s;
            s.swap(msg);
            spConn->send(s);
            spConn->shutdown();
        });
    tcpServer.start();
    loop.loop();
}