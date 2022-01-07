#pragma once
#include <xmd/types/vec3.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() plane
#define FIELDS() xmd::vec3r,origin,xmd::vec3r,normal

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    template<typename VE, typename PE>
    auto signed_dist(vec3_expr<VE> const& point, plane_expr<PE> const& plane) {
        return dot(plane.normal(), point - plane.origin());
    }

    template<typename VE, typename PE>
    auto project(vec3_expr<VE> const& point, plane_expr<PE> const& plane) {
        return point - plane.normal() * signed_dist(point, plane);
    }
}