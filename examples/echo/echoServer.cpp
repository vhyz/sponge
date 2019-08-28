#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpServer.h>
#include <iostream>

void printBufferInfo(ChannelBuffer& buffer) {
    std::cout << "capacity(): " << buffer.capacity() << std::endl;
    std::cout << "readable(): " << buffer.readable() << std::endl;
    std::cout << "readableBytes(): " << buffer.readableBytes() << std::endl;
    std::cout << "discardableBytes(): " << buffer.discardableBytes()
              << std::endl;
    std::cout << "writeable(): " << buffer.writeable() << std::endl;
    std::cout << "writeableBytes(): " << buffer.writeableBytes() << std::endl;
    std::cout << std::endl;
}
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
            printBufferInfo(msg);
            spConn->send(msg.readAllBytesAsString());
        });
    tcpServer.start();
    loop.loop();
}