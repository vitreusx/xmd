#pragma once
#include <xmd/math.h>

namespace xmd {
    template<typename VectorT>
    class box {
    public:
        VectorT cell, cell_inv;

        void set_cell(VectorT const& cell) {
            this->cell = cell;
            this->cell_inv = VectorT(1.0f/cell.x, 1.0f/cell.y, 1.0f/cell.z);
        }

        void set_cell_inv(VectorT const& cell_inv) {
            this->cell = VectorT(1.0f/cell_inv.x, 1.0f/cell_inv.y, 1.0f/cell_inv.z);
            this->cell_inv = cell_inv;
        }

        template<typename T>
        auto ray(T const& u, T const& v) {
            VectorT ray_ = v - u;
            ray_.x -= cell.x * xmd::nearbyint(ray_.x * cell_inv.x);
            ray_.y -= cell.y * xmd::nearbyint(ray_.y * cell_inv.y);
            ray_.z -= cell.z * xmd::nearbyint(ray_.z * cell_inv.z);
            return ray_;
        }
    };
}
