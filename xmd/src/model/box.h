#pragma once
#include <cmath>
#include "types/vec3.h"

namespace xmd {
    template<typename U>
    class box {
    public:
        vec3<U> cell, cell_inv;
        inline vec3<U> uv(vec3<U> const& u, vec3<U> const& v);
    };

    using boxf = box<float>;
    using boxd = box<double>;
}

#include "detail/box.inl"
