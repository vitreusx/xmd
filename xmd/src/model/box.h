#pragma once
#include <cmath>
#include "types/vec3.h"

namespace xmd {
    template<typename U>
    class box {
    public:
        vec3<U> cell, cell_inv;

        inline vec3<U> uv(vec3<U> const& u, vec3<U> const& v) {
            auto pre_uv = v - u;
            pre_uv.x -= cell.x * nearbyintf(pre_uv.x * cell_inv.x);
            pre_uv.y -= cell.y * nearbyintf(pre_uv.y * cell_inv.x);
            pre_uv.z -= cell.z * nearbyintf(pre_uv.z * cell_inv.x);
            return pre_uv;
        }
    };

    using boxf = box<float>;
    using boxd = box<double>;
}

#include "detail/box.inl"
