#pragma once
#include <xmd/types/vec3.h>
#include <Eigen/Core>

namespace xmd {
    template<typename U, typename V>
    Eigen::Matrix<U, 3, 1> convert(vec3_expr<V> const& e) {
        return { (U)e.x(), (U)e.y(), (U)e.z() };
    }

    template<typename U, typename V>
    vec3<U> convert(Eigen::Matrix<V, 3, 1> const& v) {
        return { (U)v.x(), (U)v.y(), (U)v.z() };
    }
}