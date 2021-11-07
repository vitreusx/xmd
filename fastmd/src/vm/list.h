#pragma once
#include <memory>
#include "config.h"

namespace fastmd {
    namespace detail {
        template<typename T, typename Alloc>
        class def_list_impl {
        public:
            using pointer = typename std::allocator_traits<Alloc>::pointer;
            using size_type = typename std::allocator_traits<Alloc>::size_type;

            ANY def_list_impl();
            ANY explicit def_list_impl(Alloc const& alloc);
            ANY def_list_impl(pointer data, size_type n, Alloc const& alloc = Alloc());
            ANY ~def_list_impl();

            ANY size_type size() const;
            ANY size_type capacity() const;
            ANY pointer data();

            ANY T& operator[](size_type idx);

            ANY T& push_back(T const& x);

        private:
            pointer _data;
            int _size, _capacity;
            Alloc alloc;
        };

        template<typename T, typename Alloc>
        struct list_impl {
            using type = def_list_impl<T, Alloc>;
        };
    }

    template<typename T, typename Alloc>
    using list = typename detail::list_impl<T, Alloc>::type;
}

#include "list.inl"
