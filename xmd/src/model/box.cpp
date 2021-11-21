#include "box.h"

namespace xmd {
    Eigen::Vector3d
    box::uv(const Eigen::Vector3d &u, const Eigen::Vector3d &v) const {
        Eigen::Vector3d pre_uv = v - u;
        pre_uv.x() -= cell.x() * nearbyint(pre_uv.x() * cell_inv.x());
        pre_uv.y() -= cell.y() * nearbyint(pre_uv.y() * cell_inv.x());
        pre_uv.z() -= cell.z() * nearbyint(pre_uv.z() * cell_inv.x());
        return pre_uv;
    }
}