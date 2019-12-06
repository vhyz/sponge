#pragma once
#include <cstddef>

namespace sponge {

class MutableBuffer {
   public:
    MutableBuffer() : data_(nullptr), size_(0) {}

    MutableBuffer(void* data, size_t size) : data_(data), size_(size) {}

    MutableBuffer& operator+=(size_t n) {
        size_t offset = n < size_ ? n : size_;
        data_ = static_cast<char*>(data_) + offset;
        size_ -= offset;
        return *this;
    }

    void* data() const { return data_; }

    size_t size() const { return size_; }

   protected:
    void* data_;

    size_t size_;
};

class ConstBuffer {
   public:
    ConstBuffer() : data_(nullptr), size_(0) {}

    ConstBuffer(const void* data, size_t size) : data_(data), size_(size) {}

    explicit ConstBuffer(const MutableBuffer& b)
        : data_(b.data()), size_(b.size()) {}

    ConstBuffer& operator+=(size_t n) {
        size_t offset = n < size_ ? n : size_;
        data_ = static_cast<const char*>(data_) + offset;
        size_ -= offset;
        return *this;
    }

    const void* data() const { return data_; }

    size_t size() const { return size_; }

   protected:
    const void* data_;

    size_t size_;
};

inline MutableBuffer operator+(const MutableBuffer& b, size_t n) {
    size_t offset = n < b.size() ? n : b.size();
    char* newData = static_cast<char*>(b.data()) + offset;
    size_t newSize = b.size() - offset;
    return MutableBuffer(newData, newSize);
}

inline MutableBuffer operator+(size_t n, const MutableBuffer& b) {
    return b + n;
}

inline ConstBuffer operator+(const ConstBuffer& b, size_t n) {
    size_t offset = n < b.size() ? n : b.size();
    const char* newData = static_cast<const char*>(b.data()) + offset;
    size_t newSize = b.size() - offset;
    return ConstBuffer(newData, newSize);
}

inline ConstBuffer operator+(size_t n, const ConstBuffer& b) { return b + n; }

template <typename T>
inline T BufferCast(const MutableBuffer* b) {
    return static_cast<T>(b->data());
}

template <typename T>
inline T BufferCast(const ConstBuffer* b) {
    return static_cast<T>(b->data());
}

}  // namespace sponge