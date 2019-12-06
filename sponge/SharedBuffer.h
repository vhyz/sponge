#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Buffer.h"

namespace sponge {

/*
    用std::shared_ptr管理Buffer内存
    模板类型默认使用std::vector<char>
    实际可改为std::string等支持data()与size()方法的字符容器
*/

template <typename Data>
class BasicSharedMutableBuffer : public MutableBuffer {
   public:
    using DataType = Data;

    using DataPtr = std::shared_ptr<Data>;

    BasicSharedMutableBuffer() {}

    BasicSharedMutableBuffer(const BasicSharedMutableBuffer<Data>& buffer)
        : MutableBuffer(buffer), dataPtr_(buffer.dataPtr_) {}

    explicit BasicSharedMutableBuffer(std::shared_ptr<Data> dataPtr)
        : dataPtr_(std::move(dataPtr)) {
        data_ = dataPtr_->data();
        size_ = dataPtr_->size();
    }

    void setDataPtr(std::shared_ptr<Data> dataPtr) {
        dataPtr_ = std::move(dataPtr);
        data_ = dataPtr_->data();
        size_ = dataPtr_->size();
    }

    std::shared_ptr<Data> getDataPtr() const { return dataPtr_; }

   private:
    std::shared_ptr<Data> dataPtr_;
};

template <typename Data>
class BasicSharedConstBuffer : public ConstBuffer {
   public:
    using DataType = Data;

    using DataPtr = std::shared_ptr<Data>;

    BasicSharedConstBuffer() {}

    BasicSharedConstBuffer(const BasicSharedConstBuffer& buffer)
        : ConstBuffer(buffer), dataPtr_(buffer.dataPtr_) {}

    BasicSharedConstBuffer(const BasicSharedMutableBuffer<Data>& buffer)
        : ConstBuffer(buffer), dataPtr_(buffer.getDataPtr()) {}

    explicit BasicSharedConstBuffer(std::shared_ptr<Data> dataPtr)
        : dataPtr_(std::move(dataPtr)) {
        data_ = dataPtr_->data();
        size_ = dataPtr_->size();
    }

    void setDataPtr(std::shared_ptr<Data> dataPtr) {
        dataPtr_ = std::move(dataPtr);
        data_ = dataPtr_->data();
        size_ = dataPtr_->size();
    }

    std::shared_ptr<Data> getDataPtr() const { return dataPtr_; }

   private:
    std::shared_ptr<Data> dataPtr_;
};

using SharedMutableBuffer = BasicSharedMutableBuffer<std::vector<char>>;

using SharedConstBuffer = BasicSharedConstBuffer<std::vector<char>>;

}  // namespace sponge
