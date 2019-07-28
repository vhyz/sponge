#include "TcpServer.h"

void echo(const spTcpConnection& spConn, std::string& msg) {
    std::string s;
    s.swap(msg);
    spConn->send(s);
}

int main() {
    // 首先创建一个EventLoop
    EventLoop loop;

    // 随后创建一个服务器，后两个参数分别为端口和IO线程数量
    TcpServer tcpServer(&loop, 5000, 7);

    tcpServer.setMsgCallBack(
        std::bind(&echo, std::placeholders::_1, std::placeholders::_2));

    tcpServer.start();

    auto fc = [](const spTcpConnection& spConn, std::string& msg) -> void {
        std::string s;
        s.swap(msg);
        spConn->send(s);
    };
}