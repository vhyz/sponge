#pragma once

#include <sponge/CallBack.h>
#include <sponge/TcpConnection.h>
#include <arpa/inet.h>
#include <vector>

using namespace sponge;

class LengthHeaderCodec {
   public:
    using VectorMessageCallBack = std::function<void(
        const spTcpConnection&, std::vector<int32_t>&)>;

    explicit LengthHeaderCodec(VectorMessageCallBack cb)
        : vectorMessageCallBack_(std::move(cb)) {}

    void onMessage(const spTcpConnection& spConn, ChannelBuffer& buffer) {
        while (buffer.readableBytes() >= kHeaderLen) {
            int32_t len = buffer.peekInt32();
            if (buffer.readableBytes() >= kHeaderLen + len) {
                std::vector<int32_t> nums(len);
                buffer.readNBytes(kHeaderLen);
                for (int& num : nums) {
                    num = buffer.readInt32();
                }
                vectorMessageCallBack_(spConn, nums);
            } else {
                break;
            }
        }
    }

    void send(const spTcpConnection& spConn, const std::vector<int32_t>& nums) {
        ChannelBuffer buffer;
        buffer.appendInt32(nums.size());
        for (size_t i = 0; i < nums.size(); ++i) {
            buffer.appendInt32(nums[i]);
        }
        spConn->send(buffer.getAllBytesByStringView());
    }

   private:
    VectorMessageCallBack vectorMessageCallBack_;

    const static size_t kHeaderLen = sizeof(int32_t);
};

#endif