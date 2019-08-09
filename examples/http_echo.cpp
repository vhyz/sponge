#include "../SpongeBob/TcpServer.h"

static std::string msg =
    "HTTP/1.1 200 OK\r\n"
    "Server: HttpTestServer/0.1\r\n"
    "Content-Type: text/html\r\n"
    "Connection: Keep-Alive\r\n"
    "Content-Length: 101\r\n"
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

int main() {
    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 7);
    tcpServer.setMsgCallBack(
        [](const spTcpConnection& spConn, std::string& m) {
            m.clear();
            spConn->send(msg);
        });
    tcpServer.start();
    loop.loop();
}