#pragma once
#include <xmd/meta/functors.h>
#include <tuple>
#include <string_view>

namespace xmd {
    struct generic_tag {};

#define FUNCTOR(name) name##_functor
#define IMPL(name) name##_impl

#define MAKE_FUNCTOR(name, tparam, def_value) \
    template<typename tparam, bool = std::is_base_of_v<generic_tag, tparam>>\
    struct IMPL(name) {           \
        using type = def_value;        \
        };                             \
                                       \
    template<typename tparam>    \
    using name = typename IMPL(name)<tparam>::type; \
                                              \
    using FUNCTOR(name) = xmd::as_functor<name>

#define USE_GEN_IMPL(name, gen_impl) \
    template<typename GenT, size_t... I> \
    auto name##_gen_impl_ctor(std::index_sequence<I...>) -> \
        gen_impl<GenT, I...>; \
                                          \
    template<typename GenT> \
    struct IMPL(name)<GenT, true> {                                                                  \
        static constexpr size_t num_fields = std::tuple_size_v<decltype(std::declval<GenT>().fields())>;\
        using type = decltype(name##_gen_impl_ctor<GenT>(std::make_index_sequence<num_fields>()));                                      \
    }

#define FIELDS(...) \
    inline auto fields() { \
        return std::tie(__VA_ARGS__);                \
    }               \
                    \
    inline auto fields() const {                            \
        return std::tie(__VA_ARGS__);                \
    }

#define USING_FIELD(fparam) \
    template<typename T> \
    using field = typename fparam::template type<T>

#define USING_LIFT(gen_impl, fparam) \
    template<typename OtherFunctor> \
    using lift = typename gen_impl<xmd::compose<OtherFunctor, fparam>>

#define USING_LIFTFN \
    template<template<typename> typename TypeFn> \
    using liftFn = lift<xmd::as_functor<TypeFn>>
}