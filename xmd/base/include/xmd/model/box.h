#pragma once
#include <Eigen/Core>

namespace xmd {
    class box {
    public:
        Eigen::Vector3d cell, cell_inv;
        Eigen::Vector3d uv(Eigen::Vector3d const& u,
            Eigen::Vector3d const& v) const;
    };
}
