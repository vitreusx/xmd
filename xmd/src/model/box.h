#pragma once
#include <cmath>
#include "types/vector3.h"

namespace xmd {
    template<typename U>
    class box {
    public:
        vector3<U> cell, cell_inv;
        inline vector3<U> uv(vector3<U> const& u, vector3<U> const& v);
    };

    using boxf = box<float>;
    using boxd = box<double>;
}

#include "detail/box.inl"
