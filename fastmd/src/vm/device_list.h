#pragma once
#include "config.h"

namespace fastmd {
    namespace detail {
        template<typename T>
        class def_device_list_impl {
        public:
            ANY def_device_list_impl();
            ANY def_device_list_impl(int n, T const& init);
            ANY ~def_device_list_impl();

            ANY int size() const;
            ANY int capacity() const;
            ANY T *data();

            ANY T& operator[](int idx);

            ANY T& push_back(T const& x);

        private:
            T *_data;
            int _size, _capacity;
        };

        template<typename T>
        struct device_list_impl {
            using type = def_device_list_impl<T>;
        };
    }

    template<typename T>
    using device_list = typename detail::device_list_impl<T>;
}

#include "device_list.inl"
