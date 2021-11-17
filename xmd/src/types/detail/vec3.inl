#pragma once
#include <cmath>
#include <tuple>

namespace xmd {
    template<typename U>
    vec3<U>::vec3(U&& x, U&& y, U&& z):
        x{std::forward<U>(x)},
        y{std::forward<U>(y)},
        z{std::forward<U>(z)} {}

        template<typename U>
        auto vec3<U>::fields() {
            return std::tie(x, y, z);
        }

        template<typename U>
        auto vec3<U>::fields() const {
            return std::tie(x, y, z);
        }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator=(const vec3<V> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator+=(const vec3<V> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator-=(const vec3<V> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator*=(const vec3<V> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator*=(const V &v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator/=(const vec3<V> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    auto &vec3<U>::operator/=(const V &v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    template<typename U, typename V>
    auto operator+(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x+v.x, u.y+v.y, u.z+v.z);
    }

    template<typename U, typename V>
    auto operator-(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x-v.x, u.y-v.y, u.z-v.z);
    }

    template<typename U>
    auto operator-(vec3<U> const& u) {
        return vec3(-u.x, -u.y, -u.z);
    }

    template<typename U, typename V>
    auto operator*(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x*v.x, u.y*v.y, u.z*v.z);
    }

    template<typename U, typename V>
    auto operator*(vec3<U> const& u, V const& v) {
        return vec3(u.x*v, u.y*v, u.z*v);
    }

    template<typename U, typename V>
    auto operator*(U const& u, vec3<V> const& v) {
        return vec3(u*v.x, u*v.y, u*v.z);
    }

    template<typename U, typename V>
    auto operator/(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.x/v.x, u.y/v.y, u.z/v.z);
    }

    template<typename U, typename V>
    auto operator/(vec3<U> const& u, V const& v) {
        return vec3(u.x/v, u.y/v, u.z/v);
    }

    template<typename U, typename V>
    auto operator/(U const& u, vec3<V> const& v) {
        return vec3(u/v.x, u/v.y, u/v.z);
    }

    template<typename U>
    auto norm(vec3<U> const& u) {
        return std::sqrt(norm_squared(u));
    }

    template<typename U>
    auto norm_squared(vec3<U> const& u) {
        return u.x*u.x+u.y*u.y+u.z*u.z;
    }

    template<typename U>
    auto l1_norm(vec3<U> const& u) {
        return std::abs(u.x)+std::abs(u.y)+std::abs(u.z);
    }

    template<typename U>
    auto rnorm(vec3<U> const& u) {
        return U(1) / norm(u);
    }

    template<typename U, typename V>
    auto dot(vec3<U> const& u, vec3<V> const& v) {
        return u.x*v.x+u.y*v.y+u.z*v.z;
    }

    template<typename U, typename V>
    auto cross(vec3<U> const& u, vec3<V> const& v) {
        return vec3(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.z);
    }

    template<typename U>
    auto unit(vec3<U> const& u) {
        return rnorm(u) * u;
    }
}