#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    class plane {
    public:
        vec3r origin, normal;
    };

    template<typename E>
    auto signed_dist(vec3_expr<E> const& point, plane const& plane) {
        return dot(plane.normal, point - plane.origin);
    }

    template<typename E>
    auto project(vec3_expr<E> const& point, plane const& plane) {
        return point - plane.normal * signed_dist(point, plane);
    }
}