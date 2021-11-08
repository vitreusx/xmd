#pragma once
#include <memory>
#include "tuple.h"

namespace fastmd {
    namespace detail {
        template<typename Ts, typename Allocs, size_t... ISeq>
        class joint_list_impl {
        public:
            using value_type = fastmd::tuple<
                typename std::allocator_traits<nth_type<ISeq, Allocs>>::value_type...>;
            static_assert(std::is_same_v<Ts, value_type>);
            using reference_type = fastmd::tuple<nth_type<ISeq, Ts>&...>;
            using const_reference_type = fastmd::tuple<nth_type<ISeq, Ts> const&...>;

            using pointer = fastmd::tuple<
                typename std::allocator_traits<nth_type<ISeq, Allocs>>::pointer...>;
            using size_type = std::common_type_t<
                typename std::allocator_traits<nth_type<ISeq, Allocs>>::size_type...>;

            ANY joint_list_impl() = default;
            ANY explicit joint_list_impl(Allocs const& allocs);
            ANY joint_list_impl(size_type n, pointer data,
                Allocs const& allocs = Allocs());
            ANY ~joint_list_impl();

            ANY joint_list_impl(joint_list_impl const& other);
            ANY joint_list_impl& operator=(joint_list_impl const& other);
            ANY joint_list_impl(joint_list_impl&& other);
            ANY joint_list_impl& operator=(joint_list_impl&& other);

            ANY size_type size() const;
            ANY reference_type operator[](size_type idx);
            ANY const_reference_type operator[](size_type idx) const;
            ANY reference_type append(const_reference_type const& x);

        private:
            pointer data = pointer(
                (typename std::allocator_traits<nth_type<ISeq, Allocs>>::pointer)
                nullptr...);
            size_type _size = 0, capacity = 0;
            Allocs allocs;

            template<size_t I> ANY void safe_copy(pointer to);
            template<size_t I> ANY void safe_dealloc();
        };

        template<size_t Cur, size_t Rem, typename Ts, typename Allocs, size_t... ISeq>
        struct joint_list_type_ctor {
            using type = typename joint_list_type_ctor<Cur + 1, Rem - 1, Ts, Allocs, ISeq..., Cur>::type;
        };

        template<size_t Cur, typename Ts, typename Allocs, size_t... ISeq>
        struct joint_list_type_ctor<Cur, 0, Ts, Allocs, ISeq...> {
            using type = joint_list_impl<Ts, Allocs, ISeq...>;
        };

        template<typename Ts, typename Allocs>
        using joint_list = typename joint_list_type_ctor<0, Ts::size, Ts, Allocs>::type;

        template<template<typename> typename GenAlloc, typename Ts>
        struct allocator_tuple_impl;

        template<template<typename> typename GenAlloc, typename... Ts>
        struct allocator_tuple_impl<GenAlloc, fastmd::tuple<Ts...>> {
            using type = fastmd::tuple<GenAlloc<Ts>...>;
        };

        template<template<typename> typename GenAlloc, typename Ts>
        using allocator_tuple = typename allocator_tuple_impl<GenAlloc, Ts>::type;
    }
}

#include "list.inl"
