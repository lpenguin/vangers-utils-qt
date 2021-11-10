#ifndef SPAN_H
#define SPAN_H
#include <vector>

template<typename T>
class span {
   T* ptr_;
   std::size_t len_;

public:
    span(std::vector<T>& v) noexcept
        : ptr_{v.data()}, len_(v.size())
    {}

    span(T* ptr, std::size_t len) noexcept
        : ptr_{ptr}, len_{len}
    {}

    T& operator[](int i) noexcept {
        return ptr_[i];
    }

    T const& operator[](int i) const noexcept {
        return ptr_[i];
    }

    std::size_t size() const noexcept {
        return len_;
    }

    T* begin() noexcept {
        return ptr_;
    }

    T* end() noexcept {
        return ptr_ + len_;
    }

    span<T> sub(size_t start) const noexcept {
        return span{ptr_ + start, len_ - start};
    }

    span<T> sub(size_t start, size_t length) const noexcept {
        return span{ptr_ + start, length};
    }
};
#endif // SPAN_H
