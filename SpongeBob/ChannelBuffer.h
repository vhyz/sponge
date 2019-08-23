#ifndef SPONGEBOB_CHANNELBUFFER_H
#define SPONGEBOB_CHANNELBUFFER_H

#include <vector>

/*




*/

class ChannelBuffer {
   public:
    static const size_t BufferSize = 1024;

    explicit ChannelBuffer(size_t size = BufferSize)
        : buffer_(size), readIndex_(0), writeIndex_(0) {}

    size_t readableBytes() const { return writeIndex_ - readIndex_; }

    size_t writeableBytes() const { return buffer_.size() - writeIndex_; }

    size_t discardableBytes() const { return readIndex_; }

    bool readable() const { return readIndex_ < writeIndex_; }

    bool writeable() const { return writeIndex_ < buffer_.size(); }

    

   private:
    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
};

#endif