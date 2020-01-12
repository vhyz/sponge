#include <sponge/Logger.h>
#include <sponge/TcpServer.h>
#include <iostream>

using namespace sponge;

int main() {
    setLogLevel(LOG_LEVEL_DEBUG);
    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 7);
    tcpServer.setConnCallBack([](const TcpConnection::Ptr& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");
    });
    tcpServer.setMessageCallBack(
        [](const TcpConnection::Ptr& spConn, ChannelBuffer& msg) {
            INFO("handleRead %d bytes", msg.readableBytes());
            spConn->send(msg.readPtr(), msg.readableBytes());
            msg.readAllBytes();
        });
    tcpServer.start();
    loop.loop();
}