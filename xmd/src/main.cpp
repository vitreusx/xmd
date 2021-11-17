#include <vector>
#include <iostream>
#include <tuple>
#include <string_view>

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

#define MAKE_TEMPLATE(name, def_value) \
    template<typename T>\
    struct name##_impl {           \
        using type = def_value;        \
        };                             \
                                       \
    template<typename T>    \
    using name = typename name##_impl<T>::type

#define FUNCTOR(template_name) template_name##_functor

#define MAKE_FUNCTOR(template_name) \
    struct template_name##_functor {\
        template<typename T>     \
        using type = template_name<T>; \
    }

#define GEN_IMPL(template_name) \
    template<template<typename> typename Gen, typename Functor, size_t cur, size_t rem, size_t... ISeq> \
    struct template_name##_impl_ctor {    \
        using type = typename template_name##_impl_ctor<Gen, Functor, cur+1, rem-1, cur, ISeq...>::type;                                      \
    };    \
                                          \
    template<template<typename> typename Gen, typename Functor, size_t cur, size_t... ISeq> \
    struct template_name##_impl_ctor<Gen, Functor, cur, 0, ISeq...> {                                   \
        using type = template_name##_gen_impl<Gen, Functor, ISeq...>;                                      \
    };                                    \
                                          \
    template<template<typename> typename Gen, typename Functor> \
    struct template_name##_impl<Gen<Functor>> {                                                         \
        static constexpr size_t num_fields = std::tuple_size_v<decltype(std::declval<Gen<Functor>>().fields())>;\
        using type = typename template_name##_impl_ctor<Gen, Functor, 0, num_fields>::type;                                      \
    }

template<typename Functor, typename U>
class gen_vec3 {
public:
    template<typename T>
    using field = typename Functor::template type<T>;

    field<U> x, y, z;

    gen_vec3() = default;
    gen_vec3(field<U>&& x, field<U>&& y, field<U>&& z):
        x{std::forward<field<U>>(x)},
        y{std::forward<field<U>>(y)},
        z{std::forward<field<U>>(z)} {}

    template<typename Functor2>
    explicit gen_vec3(gen_vec3<Functor2, U> const& other):
        x{other.x}, y{other.y}, z{other.z} {}

    auto fields() {
        return std::tie(x, y, z);
    }

    auto fields() const {
        return std::tie(x, y, z);
    }
};

struct identity {
    template<typename T>
    using type = T;
};

template<typename Functor1, typename Functor2>
struct compose {
    template<typename T>
    using type = typename Functor1::template type<typename Functor2::template type<T>>;
};

MAKE_TEMPLATE(ref, T&);
MAKE_FUNCTOR(ref);

template<typename U, typename V>
void setter(U& u, V const& v) {
    u = v;
}

template<template<typename> typename Gen, typename Functor, size_t... ISeq>
class ref_gen_impl {
public:
    ref_gen_impl(Gen<Functor>& x):
        inst_of_refs(std::get<ISeq>(x.fields())...) {};

    ref_gen_impl& operator=(Gen<Functor> const& x) {
        (..., setter(std::get<ISeq>(inst_of_refs.fields()), std::get<ISeq>(x.fields())));
    }

private:
    Gen<compose<ref_functor, Functor>> inst_of_refs;
};

GEN_IMPL(ref);

MAKE_TEMPLATE(cref, T const&);
MAKE_FUNCTOR(cref);

template<template<typename> typename Gen, typename Functor, size_t... ISeq>
class cref_gen_impl {
public:
    cref_gen_impl(Gen<Functor> const& x):
        inst_of_crefs(std::get<ISeq>(x.fields())...) {};

private:
    Gen<compose<cref_functor, Functor>> inst_of_crefs;
};

GEN_IMPL(cref);

MAKE_TEMPLATE(array, std::vector<T>);
MAKE_FUNCTOR(array);

template<template<typename> typename Gen, typename Functor, size_t... ISeq>
class array_gen_impl {
public:
    array_gen_impl() = default;
    array_gen_impl(size_t n, cref<Gen<Functor>> const& init = Gen<Functor>()):
        size_{n},
        inst_of_arrays{decltype(std::get<ISeq>(inst_of_arrays.fields()))(n, std::get<ISeq>(init.fields()))...} {};

    ref<Gen<Functor>> operator[](size_t idx) {
        return {std::get<ISeq>(inst_of_arrays.fields())[idx]...};
    }

    cref<Gen<Functor>> operator[](size_t idx) const {
        return {std::get<ISeq>(inst_of_arrays.fields())[idx]...};
    }

    size_t size() const {
        return size_;
    }

private:
    Gen<compose<array_functor, Functor>> inst_of_arrays;
    size_t size_;
};

GEN_IMPL(array);

template<typename U>
using vec3 = gen_vec3<identity, U>;

using vec3f = vec3<float>;

std::ostream& operator<<(std::ostream& os, vec3f const& v) {
    return (os << "(" << v.x << ", " << v.y << ", " << v.z << ")");
}

std::ostream& operator<<(std::ostream& os, array<vec3f> const& vs) {
    os << "[\n";
    for (size_t idx = 0; idx < vs.size(); ++idx) {
        os << "\t" << vs[idx] << ",\n";
    }
    os << "]";
    return os;
}

int main() {
    return 0;
}
