#include <list>

template <typename T>
class circular_buffer {
   public:
    explicit circular_buffer(size_t capacity) : capacity_(capacity) {}

    void push_back(T value) {
        if (list_.size() == capacity_) {
            list_.pop_front();
        }

        list_.push_back(std::move(value));
    }

    T& back() { return list_.back(); }

    size_t capacity() const { return capacity_; }
    
    size_t size() const { return list_.size(); }

   private:
    std::list<T> list_;

    size_t capacity_;
};