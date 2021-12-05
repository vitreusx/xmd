#pragma once
#include <xmd/utils/math.h>

namespace xmd {
    template<typename VectorT>
    class box {
    public:
        VectorT cell, cell_inv;

        void set_cell(VectorT const& cell) {
            this->cell = cell;
            this->cell_inv = VectorT(1.0f/cell.x(), 1.0f/cell.y(), 1.0f/cell.z());
        }

        void set_cell_inv(VectorT const& cell_inv) {
            this->cell = VectorT(1.0f/cell_inv.x(), 1.0f/cell_inv.y(), 1.0f/cell_inv.z());
            this->cell_inv = cell_inv;
        }

        template<typename E>
        auto warp_to_box(E const& x) {
            VectorT in_box_ = x;
            in_box_.x() -= cell.x() * xmd::nearbyint(in_box_.x() * cell_inv.x());
            in_box_.y() -= cell.y() * xmd::nearbyint(in_box_.y() * cell_inv.y());
            in_box_.z() -= cell.z() * xmd::nearbyint(in_box_.z() * cell_inv.z());
            return in_box_;
        }

        template<typename E>
        auto ray(E const& u, E const& v) {
            return warp_to_box(v - u);
        }
    };
}
