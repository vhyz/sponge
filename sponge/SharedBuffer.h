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

template <typename Data = std::vector<char>>
class SharedMutableBuffer {
   public:
    using ValueType = MutableBuffer;

    using DataType = Data;

    using DataPtr = std::shared_ptr<Data>;

    SharedMutableBuffer() {}

    explicit SharedMutableBuffer(std::shared_ptr<Data> data) {
        data_ = std::move(data);
    }

    std::shared_ptr<Data> getDataPtr() const { return data_; }

    template <typename... Args>
    SharedMutableBuffer(Args&&... args)
        : data_(std::make_shared<Data>(std::forward<Args>(args)...)),
          mutableBuffer_(data_->data(), data_->size()) {}

    const ValueType* begin() const { return mutableBuffer_.begin(); }

    const ValueType* end() const { return mutableBuffer_.end(); }

   private:
    std::shared_ptr<Data> data_;

    MutableBuffer mutableBuffer_;
};

template <typename Data = std::vector<char>>
class SharedConstBuffer {
   public:
    using ValueType = ConstBuffer;

    using DataType = Data;

    using DataPtr = std::shared_ptr<Data>;

    SharedConstBuffer() {}

    explicit SharedConstBuffer(std::shared_ptr<Data> data) {
        data_ = std::move(data);
    }
    
    std::shared_ptr<Data> getDataPtr() const { return data_; }

    template <typename... Args>
    SharedConstBuffer(Args&&... args)
        : data_(std::make_shared<Data>(std::forward<Args>(args)...)),
          constBuffer_(data_->data(), data_->size()) {}

    const ValueType* begin() const { return constBuffer_.begin(); }

    const ValueType* end() const { return constBuffer_.end(); }

   private:
    std::shared_ptr<Data> data_;

    ConstBuffer constBuffer_;
};

}  // namespace sponge
