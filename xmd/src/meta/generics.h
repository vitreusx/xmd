#pragma once
#include "meta/functors.h"
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
    struct FUNCTOR(name) {\
        template<typename tparam>     \
        using type = name<tparam>; \
    }

#define GEN_IMPL(name) name##_gen_impl

#define USE_GEN_IMPL(name) \
    template<typename GenT, size_t... I> \
    auto name##_gen_impl_ctor(std::index_sequence<I...>) -> \
        GEN_IMPL(name)<GenT, I...>; \
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

    template <typename T>
    constexpr auto get_type_name() -> std::string_view
    {
#if defined(__clang__)
        constexpr auto prefix = std::string_view{"[T = "};
        constexpr auto suffix = "]";
        constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
        constexpr auto prefix = std::string_view{"with T = "};
    constexpr auto suffix = "; ";
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__MSC_VER)
    constexpr auto prefix = std::string_view{"get_type_name<"};
    constexpr auto suffix = ">(void)";
    constexpr auto function = std::string_view{__FUNCSIG__};
#else
# error Unsupported compiler
#endif

        const auto start = function.find(prefix) + prefix.size();
        const auto end = function.find(suffix);
        const auto size = end - start;

        return function.substr(start, size);
    }
}