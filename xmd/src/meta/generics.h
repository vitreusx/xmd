#pragma once
#include "meta/functors.h"

namespace xmd {
    struct generic_tag {};

#define FUNCTOR(name) name##_functor
#define IMPL(name) name##_impl

#define MAKE_FUNCTOR(name, def_value) \
    template<typename T, bool = std::is_base_of_v<generic_tag, T>>\
    struct name##_impl {           \
        using type = def_value;        \
        };                             \
                                       \
    template<typename T>    \
    using name = typename name##_impl<T>::type; \
                                       \
    struct name##_functor {\
        template<typename T>     \
        using type = name<T>; \
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

#define FIELDS(...) \
    using field_types = decltype(std::make_tuple(__VA_ARGS__));                               \
                                    \
    inline auto fields() {          \
        return std::tie(__VA_ARGS__);                                \
    }                               \
                                    \
    inline auto fields() const {    \
        return std::tie(__VA_ARGS__);                                \
    }

#define GENERIC_FROM_MEMBER(member) \
    using field_types = typename decltype(member)::field_types;                               \
                                    \
    inline auto fields() {          \
        return member.fields();                                \
    }                               \
                                    \
    inline auto fields() const {    \
        return member.fields();                                \
    }                               \
                                    \
    template<typename Functor>      \
    using lift = typename decltype(member)::template lift<Functor>
}