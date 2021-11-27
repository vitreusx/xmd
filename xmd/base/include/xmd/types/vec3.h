#pragma once
#include <xmd/math.h>

namespace xmd {
    template<typename U>
    class vec3 {
    public:
        U x, y, z;

        vec3() = default;

        vec3(U x, U y, U z) :
            x{x}, y{y}, z{z} {};

        template<typename V>
        vec3(vec3<V> const& v):
            x{v.x}, y{v.y}, z{v.z} {};

        static vec3<U> Zero() {
            return { 0.0f, 0.0f, 0.0f };
        }

    public:
        template<typename V>
        auto &operator=(vec3<V> const &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        template<typename V>
        auto &operator+=(vec3<V> const &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        template<typename V>
        auto &operator-=(vec3<V> const &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        template<typename V>
        auto &operator*=(vec3<V> const &v) {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        template<typename S>
        auto &operator*=(S const &s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        template<typename V>
        auto &operator/=(vec3<V> const &v) {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        template<typename S>
        auto &operator/=(S const &s) {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }
    };

    template<typename U, typename V>
    auto operator+(vec3<U> const &u, vec3<V> const &v) {
        return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
    }

    template<typename U, typename V>
    auto operator-(vec3<U> const &u, vec3<V> const &v) {
        return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
    }

    template<typename U>
    auto operator-(vec3<U> const &u) {
        return vec3(-u.x, -u.y, -u.z);
    }

    template<typename U, typename V>
    auto operator*(vec3<U> const &u, vec3<V> const &v) {
        return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
    }

    template<typename U, typename S>
    auto operator*(vec3<U> const &u, S const &s) {
        return vec3(u.x * s, u.y * s, u.z * s);
    }

    template<typename S, typename V>
    auto operator*(S const &s, vec3<V> const &v) {
        return vec3(s * v.x, s * v.y, s * v.z);
    }

    template<typename U, typename V>
    auto operator/(vec3<U> const &u, vec3<V> const &v) {
        return vec3(u.x / v.x, u.y / v.y, u.z / v.z);
    }

    template<typename U, typename S>
    auto operator/(vec3<U> const &u, S const &s) {
        return vec3(u.x / s, u.y / s, u.z / s);
    }

    template<typename S, typename V>
    auto operator/(S const &s, vec3<V> const &v) {
        return vec3(s / v.x, s / v.y, s / v.z);
    }

    template<typename U>
    auto norm(vec3<U> const &u) {
        return xmd::sqrt(norm_squared(u));
    }

    template<typename U>
    auto norm_squared(vec3<U> const &u) {
        return u.x * u.x + u.y * u.y + u.z * u.z;
    }

    template<typename U>
    auto l1_norm(vec3<U> const &u) {
        return xmd::abs(u.x) + xmd::abs(u.y) + xmd::abs(u.z);
    }

    template<typename U>
    auto norm_inv(vec3<U> const &u) {
        return U(1.0f) / norm(u);
    }

    template<typename U, typename V>
    auto dot(vec3<U> const &u, vec3<V> const &v) {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    template<typename U, typename V>
    auto cross(vec3<U> const &u, vec3<V> const &v) {
        auto s_x = u.y * v.z - u.z * v.y;
        auto s_y = u.z * v.x - u.x * v.z;
        auto s_z = u.x * v.y - u.y * v.x;
        return vec3(s_x, s_y, s_z);
    }

    template<typename U>
    auto unit(vec3<U> const &u) {
        return norm_inv(u) * u;
    }

    using vec3f = vec3<float>;

    template<typename U>
    class vec3_array {
    public:
        vec3_array() = default;
        vec3_array(U *x, U *y, U *z, int size):
            x{x}, y{y}, z{z}, size{size} {};

        vec3<U&> operator[](int idx) const {
            return { x[idx], y[idx], z[idx] };
        }

        U * x = nullptr;
        U * y = nullptr;
        U * z = nullptr;
        int size = 0;
    };

    using vec3f_array = vec3_array<float>;
}
