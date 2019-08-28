#include <iostream>
#include "../SpongeBob/TcpServer.h"

int main() {
    std::string msg =
        "HTTP/1.1 200 OK\r\n"
        "Server: HttpTestServer/0.1\r\n"
        "Content-Type: text/html\r\n"
        "Connection: Keep-Alive\r\n"
        "Content-Length: 100101\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>SpongeBob</title>"
        "</head>"
        "<body>"
        "<h1>HttpTestServer</h1>"
        "</body>"
        "</html>";
    msg += std::string(100000, 'a');

    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 7);
    std::cout << msg.size();
    tcpServer.setMessageCallBack(
        [&](const spTcpConnection& spConn, ChannelBuffer& buffer) {
            buffer.readAllBytes();
            spConn->send(msg);
        });
    tcpServer.start();
    loop.loop();
}