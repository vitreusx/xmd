#pragma once
#include "meta/functors.h"
#include <tuple>

namespace xmd {
    struct generic_tag {};

#define FUNCTOR(name) name##_functor
#define IMPL(name) name##_impl

#define MAKE_FUNCTOR(name, tparam, def_value) \
    template<typename tparam, bool = std::is_base_of_v<generic_tag, tparam>>\
    struct name##_impl {           \
        using type = def_value;        \
        };                             \
                                       \
    template<typename tparam>    \
    using name = typename name##_impl<tparam>::type; \
                                       \
    struct name##_functor {\
        template<typename tparam>     \
        using type = name<tparam>; \
    }

#define GEN_IMPL(name) name##_gen_impl

#define USE_GEN_IMPL(name) \
    template<typename GenT, size_t... I> \
    auto name##_gen_impl_ctor(std::index_sequence<I...>) -> \
        name##_gen_impl<GenT, I...>; \
                                          \
    template<typename GenT> \
    struct name##_impl<GenT, true> {                                                                  \
        static constexpr size_t num_fields = std::tuple_size_v<decltype(std::declval<GenT>().fields())>;\
        using type = decltype(name##_gen_impl_ctor<GenT>(std::make_index_sequence<num_fields>()));                                      \
    }
}