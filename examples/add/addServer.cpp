#include <SpongeBob/EventLoop.h>
#include <SpongeBob/Logger.h>
#include <SpongeBob/TcpServer.h>
#include <SpongeBob/ThreadPool.h>

int main() {
    EventLoop loop;
    TcpServer tcpServer(&loop, 5000, 3);
    ThreadPool pool(10);
    tcpServer.setMessageCallBack(
        [&](const spTcpConnection& spConn, std::string& msg) {
            if (msg.size() != 8) {
                INFO("size != 8");
                return;
            }
            INFO(msg.c_str());
            pool.putTask([spConn, msg]() {
                std::string a = msg.substr(0, 4);
                std::string b = msg.substr(4, 4);
                int numa = std::atoi(a.c_str());
                int numb = std::atoi(b.c_str());
                std::string res = std::to_string(numa + numb);
                INFO(res.c_str());
                spConn->send(std::move(res));
            });
            msg.clear();
        });
    tcpServer.start();
    pool.start();
    loop.loop();
}