#include <sponge/Logger.h>
#include <sponge/TcpServer.h>
#include <sponge/ThreadPool.h>
#include "codec.h"

#include <algorithm>

using namespace std::placeholders;
using namespace sponge;

class SortServer {
   public:
    SortServer(EventLoop* loop, uint16_t port, int numThreads)
        : tcpServer_(loop, port, numThreads),
          codec_(std::bind(&SortServer::onMessage, this, _1, _2)),
          threadPool_(5) {
        tcpServer_.setConnCallBack(
            std::bind(&SortServer::onConnection, this, _1));
        tcpServer_.setMessageCallBack(
            std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2));
    }

    void onConnection(const TcpConnection::Ptr& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");
    }
    
    void onMessage(const TcpConnection::Ptr& spConn, std::vector<int32_t>& nums) {
        threadPool_.putTask(
            std::bind(&SortServer::work, this, spConn, std::move(nums)));
    }

    void start() {
        threadPool_.start();
        tcpServer_.start();
    }

   private:
    void work(std::weak_ptr<TcpConnection> wpConn, std::vector<int32_t>& nums) {
        if (auto spConn = wpConn.lock()) {
            std::sort(nums.begin(), nums.end());
            codec_.send(spConn, nums);
        }
    }

    TcpServer tcpServer_;

    LengthHeaderCodec codec_;

    ThreadPool threadPool_;
};