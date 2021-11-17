#include <iostream>
#include <string>
#include <utility>
#include <tuple>
#include "meta/generics.h"
#include "meta/ref.h"

template<typename U>
struct vec3: public xmd::generic_tag {
    U x, y, z;

    vec3() = default;
    vec3(U&& x, U&& y, U&& z):
        x{std::forward<U>(x)},
        y{std::forward<U>(y)},
        z{std::forward<U>(z)} {};

    FIELDS(x, y, z);

    template<typename Functor>
    using lift = vec3<xmd::apply<Functor, U>>;
};

using vec3f = vec3<float>;

std::ostream& operator<<(std::ostream& os, vec3f const& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

int main() {
    vec3f v1(1.0f, 2.0f, 3.0f), v2(4.0f, 5.0f, 6.0f);
    std::cout << v1 << '\n';
    xmd::ref<vec3f> vref(v1);
    vref = xmd::const_ref<vec3f>(v2);
    std::cout << v1 << '\n';
    return 0;
}
