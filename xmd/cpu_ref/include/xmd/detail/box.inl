#pragma once

namespace xmd {
    template<typename U>
    vec3<U> box<U>::uv(vec3<U> const& u, vec3<U> const& v) const {
        auto pre_uv = v - u;
        pre_uv.x -= cell.x * nearbyintf(pre_uv.x * cell_inv.x);
        pre_uv.y -= cell.y * nearbyintf(pre_uv.y * cell_inv.x);
        pre_uv.z -= cell.z * nearbyintf(pre_uv.z * cell_inv.x);
        return pre_uv;
    }
}