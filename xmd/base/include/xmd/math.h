#pragma once
#include <cmath>
#include <Eigen/Core>

namespace xmd {
    template<typename T>
    T sqrt(T const& x) {
        return (T)std::sqrt(x);
    }

    template<typename T>
    T cbrt(T const& x) {
        return (T)std::cbrt(x);
    }

    template<size_t N, typename T>
    T ipow(T const& x) {
        if constexpr (N == 1) return x;
        else return x * ipow<N-1, T>(x);
    }

    template<typename T>
    T nearbyint(T x) {
        return (T)std::nearbyint(x);
    }

    template<typename T>
    T abs(T const& x) {
        return (T)std::abs(x);
    }

    inline Eigen::Vector3d unit(Eigen::Vector3d const& v) {
        return v.normalized();
    }

    template<typename T>
    T cos(T const& x) {
        return (T)std::cos(x);
    }

    template<typename T>
    T sin(T const& x) {
        return (T)std::sin(x);
    }

    template<typename T>
    T acos(T const& x) {
        return (T)std::acos(x);
    }
}
