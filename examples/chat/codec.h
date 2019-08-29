#ifndef SPONGEBOB_EXAMPLES_CHAT_CODEC
#define SPONGEBOB_EXAMPLES_CHAT_CODEC

#include <SpongeBob/TcpConnection.h>
#include <functional>
#include <string>

class LengthHeaderCodec {
   public:
    using StringMessageCallBack =
        std::function<void(const spTcpConnection& spConn, const std::string&)>;

    explicit LengthHeaderCodec(StringMessageCallBack cb)
        : messageCallBack_(std::move(cb)) {}

    void onMessage(const spTcpConnection& spConn, ChannelBuffer& buffer) {
        while (buffer.readableBytes() >= kHeaderLen) {
            int32_t len = buffer.peekInt32();
            if (buffer.readableBytes() >= len + kHeaderLen) {
                buffer.readNBytes(kHeaderLen);
                messageCallBack_(spConn, buffer.readNBytesAsString(len));
            } else {
                break;
            }
        }
    }

    void send(const spTcpConnection& spConn, const std::string& msg) {
        ChannelBuffer buffer;
        buffer.appendInt32(msg.size());
        buffer.append(msg.data(), msg.size());
        spConn->send(buffer.readPtr(), buffer.readableBytes());
    }

   private:
    static const size_t kHeaderLen = sizeof(int32_t);

    StringMessageCallBack messageCallBack_;
};

#endif