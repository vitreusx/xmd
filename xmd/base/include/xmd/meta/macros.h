#pragma once
#include "functors.h"
#include <type_traits>
#include <tuple>

namespace xmd {
    template<typename T>
    class generic {};

#define FUNCTOR_DECL(name, tparam, defval) \
    template<typename tparam>              \
    struct name##_impl {                   \
        using type = defval;                                       \
    };                                     \
    \
    template<typename tparam>              \
    using name = typename name##_impl<tparam>::type;               \
                                           \
    using name##_functor = xmd::as_functor<name>

#define USE_GEN_IMPL(name, gen_impl) \
    template<typename GenT, size_t... I> \
    auto name##_gen_impl_helper(std::index_sequence<I...>) -> \
        gen_impl<GenT, I...>;        \
                                     \
        template<typename GenT>      \
        struct name##_impl<generic<GenT>> {                   \
            using type = decltype(name##_gen_impl_helper<GenT>(std::make_index_sequence<GenT::NUM_FIELDS>()));                             \
        }

#define MEM_TYPE(type, name) xmd::apply<Functor_, type>
#define MEM_NAME(type, name) name##_
#define MEM_DECL(type, name) MEM_TYPE(type, name) MEM_NAME(type, name)
#define ARG(type, name) MEM_TYPE(type, name) MEM_NAME(type, name)
#define NORMAL_CTOR_SETTER(type, name) MEM_NAME(type, name){MEM_NAME(type, name)}
#define EXPR_FN_NAME(type, name) name
#define EXPR_FN(type, name) \
    auto EXPR_FN_NAME(type, name)() const { \
        return static_cast<E const&>(*this).EXPR_FN_NAME(type, name)(); \
    }
#define EXPR_CTOR_SETTER(type, name) MEM_NAME(type, name){e.EXPR_FN_NAME(type, name)()}
#define EXPR_FN_IMPL(type, name) \
    auto const& EXPR_FN_NAME(type, name)() const { \
        return MEM_NAME(type, name);        \
    }

#define MEM_TYPES_1(_1) MEM_TYPE _1
#define MEM_NAMES_1(_1) MEM_NAME _1
#define MEM_DECLS_1(_1) MEM_DECL _1
#define ARGS_1(_1) ARG _1
#define NORMAL_CTOR_SETTERS_1(_1) NORMAL_CTOR_SETTER _1
#define EXPR_FNS_1(_1) EXPR_FN _1
#define EXPR_CTOR_SETTERS_1(_1) EXPR_CTOR_SETTER _1
#define EXPR_FN_IMPLS_1(_1) EXPR_FN_IMPL _1

#define MEM_TYPES_2(_1, ...)\
    MEM_TYPE _1, MEM_TYPES_1(__VA_ARGS__)
#define MEM_NAMES_2(_1, ...)\
    MEM_NAME _1, MEM_NAMES_1(__VA_ARGS__)
#define MEM_DECLS_2(_1, ...)\
    MEM_DECL _1;\
    MEM_DECLS_1(__VA_ARGS__)
#define ARGS_2(_1, ...)\
    ARG _1, ARGS_1(__VA_ARGS__)
#define NORMAL_CTOR_SETTERS_2(_1, ...)\
    NORMAL_CTOR_SETTER _1, NORMAL_CTOR_SETTERS_1(__VA_ARGS__)
#define EXPR_FNS_2(_1, ...)\
    EXPR_FN _1;\
    EXPR_FNS_1(__VA_ARGS__)
#define EXPR_CTOR_SETTERS_2(_1, ...) \
    EXPR_CTOR_SETTER _1, EXPR_CTOR_SETTERS_1(__VA_ARGS__)
#define EXPR_FN_IMPLS_2(_1, ...) \
    EXPR_FN_IMPL _1;             \
    EXPR_FN_IMPLS_1(__VA_ARGS__)

#define MEM_TYPES_3(_1, ...)\
    MEM_TYPE _1, MEM_TYPES_2(__VA_ARGS__)
#define MEM_NAMES_3(_1, ...)\
    MEM_NAME _1, MEM_NAMES_2(__VA_ARGS__)
#define MEM_DECLS_3(_1, ...)\
    MEM_DECL _1;\
    MEM_DECLS_2(__VA_ARGS__)
#define ARGS_3(_1, ...)\
    ARG _1, ARGS_2(__VA_ARGS__)
#define NORMAL_CTOR_SETTERS_3(_1, ...)\
    NORMAL_CTOR_SETTER _1, NORMAL_CTOR_SETTERS_2(__VA_ARGS__)
#define EXPR_FNS_3(_1, ...)\
    EXPR_FN _1;\
    EXPR_FNS_2(__VA_ARGS__)
#define EXPR_CTOR_SETTERS_3(_1, ...) \
    EXPR_CTOR_SETTER _1, EXPR_CTOR_SETTERS_2(__VA_ARGS__)
#define EXPR_FN_IMPLS_3(_1, ...) \
    EXPR_FN_IMPL _1;             \
    EXPR_FN_IMPLS_2(__VA_ARGS__)

#define EXPR_DECL(N, name, ...) \
    template<typename E>        \
    class name##_expr { \
    public:                     \
        EXPR_FNS_##N(__VA_ARGS__);\
    }

#define GEN_CLASS_DECL(N, name, ...) \
    template<typename Functor_>        \
    class gen_##name: public xmd::generic<gen_##name<Functor_>>, \
        name##_expr<gen_##name<Functor_>> {\
    public:                     \
        MEM_DECLS_##N(__VA_ARGS__);\
        explicit inline gen_##name(ARGS_##N(__VA_ARGS__)):   \
            NORMAL_CTOR_SETTERS_##N(__VA_ARGS__) {};                         \
                               \
        template<typename E>   \
        inline gen_##name(name##_expr<E> const& e):               \
            EXPR_CTOR_SETTERS_##N(__VA_ARGS__) {};\
                               \
        \
        using field_types = std::tuple<MEM_TYPES_##N(__VA_ARGS__)>;\
        \
        template<int I>\
        using field_type = std::tuple_element_t<I, field_types>;\
        \
        auto fields() {\
        return std::tie(MEM_NAMES_##N(__VA_ARGS__));\
        }\
        \
        template<int I>\
        auto field() {\
        return std::get<I>(this->fields());             \
        }                \
        \
        auto fields() const {    \
        return std::tie(MEM_NAMES_##N(__VA_ARGS__));    \
        }\
                                 \
        template<int I>          \
        auto field() const {           \
        return std::get<I>(this->fields());\
        }                            \
                                     \
        EXPR_FN_IMPLS_##N(__VA_ARGS__);                             \
                                 \
        template<typename F2>    \
        using lift = gen_##name<xmd::compose<F2, Functor_>>;\
    }

#define GEN_DECL(...) \
    EXPR_DECL(__VA_ARGS__); \
    GEN_CLASS_DECL(__VA_ARGS__)
}
