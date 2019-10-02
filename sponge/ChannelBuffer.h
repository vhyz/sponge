#pragma once

#include <assert.h>
#include <endian.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <string_view>
#include <vector>

namespace sponge {

class ChannelBuffer {
   private:
    static const size_t kInitBufferSize = 1024;

   public:

    // init ChannelBuffer with size
    explicit ChannelBuffer(size_t size = kInitBufferSize)
        : buffer_(size), readIndex_(0), writeIndex_(0) {}

    size_t readableBytes() const { return writeIndex_ - readIndex_; }

    size_t writeableBytes() const { return buffer_.size() - writeIndex_; }

    size_t discardableBytes() const { return readIndex_; }

    bool readable() const { return readIndex_ < writeIndex_; }

    bool writeable() const { return writeIndex_ < buffer_.size(); }

    // should return size() but not capcaity()
    // because we can only use size of a vector
    size_t capacity() { return buffer_.size(); }

    char* writePtr() { return buffer_.data() + writeIndex_; }

    const char* writePtr() const { return buffer_.data() + writeIndex_; }

    const char* readPtr() const { return buffer_.data() + readIndex_; }

    int64_t peekInt64() const {
        int64_t n;
        memcpy(&n, readPtr(), sizeof(int64_t));
        return be64toh(n);
    }

    int32_t peekInt32() const {
        int32_t n;
        memcpy(&n, readPtr(), sizeof(int32_t));
        return be32toh(n);
    }

    int16_t peekInt16() const {
        int16_t n;
        memcpy(&n, readPtr(), sizeof(int16_t));
        return be16toh(n);
    }

    int8_t peekInt8() const {
        int8_t n = *readPtr();
        return n;
    }

    int64_t readInt64() {
        int64_t n = peekInt64();
        readNBytes(sizeof(int64_t));
        return n;
    }

    int32_t readInt32() {
        int32_t n = peekInt32();
        readNBytes(sizeof(int32_t));
        return n;
    }

    int16_t readInt16() {
        int16_t n = peekInt16();
        readNBytes(sizeof(int16_t));
        return n;
    }

    int8_t readInt8() {
        int8_t n = peekInt8();
        readNBytes(sizeof(int8_t));
        return n;
    }

    void appendInt64(int64_t n) {
        int64_t be = htobe64(n);
        append(&be, sizeof(int64_t));
    }

    void appendInt32(int32_t n) {
        int32_t be = htobe32(n);
        append(&be, sizeof(int32_t));
    }

    void appendInt16(int16_t n) {
        int16_t be = htobe16(n);
        append(&be, sizeof(int16_t));
    }

    void appendInt8(int8_t n) { append(&n, sizeof(int8_t)); }

    void readNBytes(size_t n) {
        assert(readableBytes() >= n);

        if (readableBytes() > n) {
            readIndex_ += n;
        } else {
            readAllBytes();
        }
    }

    void readAllBytes() {
        readIndex_ = 0;
        writeIndex_ = 0;
    }

    std::string readNBytesAsString(size_t n) {
        std::string str(readPtr(), n);
        readNBytes(n);
        return str;
    }

    std::string readAllBytesAsString() {
        return readNBytesAsString(readableBytes());
    }

    std::string_view getNBytesByStringView(size_t n) const {
        return std::string_view(readPtr(), n);
    }

    std::string_view getAllBytesByStringView() const {
        return getNBytesByStringView(readableBytes());
    }

    // 请求移除未使用的容量
    void shrink(size_t reserve = 0) {
        ChannelBuffer other;
        other.ensureWriteableBytes(readableBytes() + reserve);
        other.append(getAllBytesByStringView());
        this->swap(other);
    }

    void swap(ChannelBuffer& other) {
        buffer_.swap(other.buffer_);
        std::swap(readIndex_, other.readIndex_);
        std::swap(writeIndex_, other.writeIndex_);
    }

    void append(const void* buf, size_t len) {
        ensureWriteableBytes(len);
        const char* ptr = static_cast<const char*>(buf);
        std::copy(ptr, ptr + len, writePtr());

        assert(writeIndex_ + len <= capacity());
        writeIndex_ += len;
    }

    void append(std::string_view buf) { append(buf.data(), buf.size()); }

    void ensureWriteableBytes(size_t len) {
        if (writeableBytes() < len) {
            if (discardableBytes() + writeableBytes() > len) {
                // move data to front

                size_t readable = readableBytes();
                char* begin = buffer_.data();
                std::copy(begin + readIndex_, begin + writeIndex_, begin);
                readIndex_ = 0;
                writeIndex_ = readable;
            } else {
                // vector has not enough space
                // it will use capacity or get new memory
                // it depends on the length of vector's capacity

                buffer_.resize(writeIndex_ + len);
            }
        }
    }

   private:
    std::vector<char> buffer_;

    size_t readIndex_;

    size_t writeIndex_;
};

}  // namespace sponge
