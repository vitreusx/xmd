#pragma once
#include <vector>

namespace xmd {
    template<typename T>
    class array {
    public:
        inline array() = default;
        inline array(T *data, size_t size);

        inline T& operator[](size_t idx);
        inline T const& operator[](size_t idx) const;
        inline size_t size() const;

    private:
        T *data_ = nullptr;
        size_t size_ = 0;
    };
}