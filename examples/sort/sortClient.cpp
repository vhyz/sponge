#include <sponge/Logger.h>
#include <sponge/TcpClient.h>
#include <stdlib.h>
#include <iostream>
#include "codec.h"

using namespace std::placeholders;
using namespace sponge;

class SortClient {
   public:
    SortClient(EventLoop* loop, const InetAddress& serverAddress)
        : loop_(loop),
          client_(loop, serverAddress),
          codec_(std::bind(&SortClient::onMessage, this, _1, _2)) {
        srand(time(0));
        client_.setConnectionCallBack(
            std::bind(&SortClient::onConnection, this, _1));
        client_.setMessageCallBack(
            std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2));
    }

    void onConnection(const spTcpConnection& spConn) {
        INFO("connection %s -> %s %s",
             spConn->getPeerAddr().getIpAndPort().c_str(),
             spConn->getLocalAddr().getIpAndPort().c_str(),
             spConn->isConnected() ? "up" : "down");

        if (spConn->isConnected()) {
            conn_ = spConn;
            sendTimer_ = loop_->runEvery(1, std::bind(&SortClient::send, this));
        } else {
            loop_->cancel(sendTimer_);
            conn_.reset();
        }
    }

    void onMessage(const spTcpConnection& spConn, std::vector<int32_t>& nums) {
        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[i - 1] > nums[i]) {
                std::cout << "sort error\n";
                return;
            }
        }
        std::cout << "sort completely\n";
    }

    void send() {
        size_t len = 1000 * 1000;
        std::vector<int32_t> nums(len);
        for (size_t i = 0; i < len; ++i) {
            nums[i] = rand();
        }
        codec_.send(conn_, nums);
    }

   private:
    EventLoop* loop_;

    TcpClient client_;

    LengthHeaderCodec codec_;

    spTcpConnection conn_;

    TimerId sendTimer_;
};