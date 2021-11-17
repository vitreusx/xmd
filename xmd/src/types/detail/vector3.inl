#pragma once

namespace xmd {
    template<typename U>
    vector3<U>::vector3(U x, U y, U z):
        x{x}, y{y}, z{z} {}

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator=(const vector3<V> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator+=(const vector3<V> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator-=(const vector3<V> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator*=(const vector3<V> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator*=(const V &v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator/=(const vector3<V> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    template<typename U>
    template<typename V>
    vector3<U> &vector3<U>::operator/=(const V &v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    template<typename U, typename V>
    auto operator+(vector3<U> const& u, vector3<V> const& v) {
        return vector3(u.x+v.x, u.y+v.y, u.z+v.z);
    }

    template<typename U, typename V>
    auto operator-(vector3<U> const& u, vector3<V> const& v) {
        return vector3(u.x-v.x, u.y-v.y, u.z-v.z);
    }

    template<typename U>
    auto operator-(vector3<U> const& u) {
        return vector3(-u.x, -u.y, -u.z);
    }

    template<typename U, typename V>
    auto operator*(vector3<U> const& u, vector3<V> const& v) {
        return vector3(u.x*v.x, u.y*v.y, u.z*v.z);
    }

    template<typename U, typename V>
    auto operator*(vector3<U> const& u, V const& v) {
        return vector3(u.x*v, u.y*v, u.z*v);
    }

    template<typename U, typename V>
    auto operator*(U const& u, vector3<V> const& v) {
        return vector3(u*v.x, u*v.y, u*v.z);
    }

    template<typename U, typename V>
    auto operator/(vector3<U> const& u, vector3<V> const& v) {
        return vector3(u.x/v.x, u.y/v.y, u.z/v.z);
    }

    template<typename U, typename V>
    auto operator/(vector3<U> const& u, V const& v) {
        return vector3(u.x/v, u.y/v, u.z/v);
    }

    template<typename U, typename V>
    auto operator/(U const& u, vector3<V> const& v) {
        return vector3(u/v.x, u/v.y, u/v.z);
    }

    template<typename U>
    auto norm(vector3<U> const& u) {
        return std::sqrt(norm_squared(u));
    }

    template<typename U>
    auto norm_squared(vector3<U> const& u) {
        return u.x*u.x+u.y*u.y+u.z*u.z;
    }

    template<typename U>
    auto l1_norm(vector3<U> const& u) {
        return std::abs(u.x)+std::abs(u.y)+std::abs(u.z);
    }

    template<typename U>
    auto rnorm(vector3<U> const& u) {
        return U(1) / norm(u);
    }

    template<typename U, typename V>
    auto dot(vector3<U> const& u, vector3<V> const& v) {
        return u.x*v.x+u.y*v.y+u.z*v.z;
    }

    template<typename U, typename V>
    auto cross(vector3<U> const& u, vector3<V> const& v) {
        auto p = u.y*v.z-u.z*v.y;
        auto q = u.z*v.x-u.x*v.z;
        auto r = u.x*v.y-u.y*v.z;
        return vector3(p, q, r);
    }

    template<typename U>
    auto unit(vector3<U> const& u) {
        return rnorm(u) * u;
    }
}

namespace xmd {
    template<typename U>
    vector3<U&> vector3_array<U>::operator[](size_t idx) {
        return { x[idx], y[idx], z[idx] };
    }

    template<typename U>
    vector3<U> vector3_array<U>::operator[](size_t idx) const {
        return { x[idx], y[idx], z[idx] };
    }
}

namespace xmd::detail {
    template<typename U>
    struct array_impl<vector3<U>> {
        using type = vector3_array<U>;
    };
}
