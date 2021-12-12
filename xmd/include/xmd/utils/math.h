#pragma once
#include <cmath>
#include <Eigen/Core>

namespace xmd {
    template<typename T>
    auto sqrt(T const& x) {
        return std::sqrt(x);
    }

    template<typename T>
    auto cbrt(T const& x) {
        return std::cbrt(x);
    }

    template<size_t N, typename T>
    auto ipow(T const& x) {
        if constexpr (N == 1) return x;
        else return x * ipow<N-1, T>(x);
    }

    template<typename T>
    auto nearbyint(T x) {
        return std::nearbyint(x);
    }

    template<typename T>
    auto abs(T const& x) {
        return std::abs(x);
    }

    inline Eigen::Vector3d unit(Eigen::Vector3d const& v) {
        return v.normalized();
    }

    template<typename T>
    auto cos(T const& x) {
        return std::cos(x);
    }

    template<typename T>
    auto sin(T const& x) {
        return std::sin(x);
    }

    template<typename T>
    auto acos(T const& x) {
        auto x_ = x < T(-1.0) ? T(-1.0) : (x > T(1.0) ? T(1.0) : x);
        return std::acos(x_);
    }

    template<typename T>
    auto exp(T const& x) {
        return std::exp(x);
    }

    template<typename T1, typename T2>
    auto min(T1 const& x1, T2 const& x2) {
        return std::min(x1, x2);
    }

    template<typename T1, typename T2>
    auto max(T1 const& x1, T2 const& x2) {
        return std::max(x1, x2);
    }

    template<typename T>
    auto ceil(T const& x) {
        return std::ceil(x);
    }

    template<typename T>
    auto log(T const& x) {
        return std::log(x);
    }

    template<typename T>
    auto isnan(T const& x) {
        return std::isnan(x);
    }

    template<typename T>
    auto adjust(T value, T min_value, T max_value) {
        return value < min_value ? min_value : (value < max_value ? value : max_value);
    }
}
