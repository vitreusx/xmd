#include <iostream>
#include <string>
#include <utility>
#include <tuple>
#include "meta/generics.h"
#include "meta/ref.h"
#include "meta/const_ref.h"
#include "meta/list.h"
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

template<typename U>
struct vec3: public xmd::generic_tag {
    U x, y, z;

    vec3() = default;
    vec3(U&& x, U&& y, U&& z):
        x{std::forward<U>(x)},
        y{std::forward<U>(y)},
        z{std::forward<U>(z)} {};

    using field_types = std::tuple<U, U, U>;

    auto fields() {
        return std::tie(x, y, z);
    }

    auto fields() const {
        return std::tie(x, y, z);
    }

    template<typename Functor>
    using lift = vec3<xmd::apply<Functor, U>>;
};

using vec3f = vec3<float>;

std::ostream& operator<<(std::ostream& os, xmd::const_ref<vec3f> const& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, xmd::list<vec3f> const& vs) {
    os << "[\n";
    for (size_t i = 0; i < vs.size(); ++i) {
        os << "\t" << vs[i] << ",\n";
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& vs) {
    os << "[\n";
    for (size_t i = 0; i < vs.size(); ++i) {
        os << "\t" << vs[i] << ",\n";
    }
    os << "]";
    return os;
}

int main() {
    xmd::list<vec3f> vs(10, vec3f(1.0f, 2.0f, 3.0f));
    std::cout << vs << '\n';
    vs.push_back(vec3f(4.0f, 5.0f, 6.0f));
    std::cout << vs << '\n';

    return 0;
}
