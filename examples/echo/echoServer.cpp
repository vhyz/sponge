#include <sponge/Logger.h>
#include <sponge/TcpServer.h>
#include <iostream>

using namespace sponge;

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
        [](const spTcpConnection& spConn, ChannelBuffer& msg) {
            INFO("handleRead %d bytes", msg.readableBytes());
            spConn->send(msg.readAllBytesAsString());
        });
    tcpServer.start();
    loop.loop();
}