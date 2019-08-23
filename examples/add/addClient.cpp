#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpClient.h>
int main() {
    EventLoop loop;
    InetAddress addr("127.0.0.1", 5000);
    TcpClient tcpClient(&loop, addr);
    tcpClient.setMessageCallBack([](const spTcpConnection& spConn,
                                    std::string& msg) { INFO(msg.c_str()); });
    tcpClient.setConnectionCallBack([](const spTcpConnection& spConn) {
        if (spConn->isConnected()) {
            int a = 1111;
            int b = 2222;
            spConn->send(std::to_string(a) + std::to_string(b));
        }
    });
    tcpClient.start();
    loop.loop();
}