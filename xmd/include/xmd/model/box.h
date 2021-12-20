#pragma once
#include <xmd/utils/math.h>

namespace xmd {
    template<typename VectorT>
    class box {
    public:
        VectorT cell, cell_inv;

        box(): cell{0.0, 0.0, 0.0}, cell_inv{0.0, 0.0, 0.0} {};

        void set_cell(VectorT const& cell) {
            this->cell = cell;

            cell_inv = VectorT::Zero();
            if (cell.x() != 0.0) cell_inv.x() = 1.0 / cell.x();
            if (cell.y() != 0.0) cell_inv.y() = 1.0 / cell.y();
            if (cell.z() != 0.0) cell_inv.z() = 1.0 / cell.z();
        }

        void set_cell_inv(VectorT const& cell_inv) {
            cell = VectorT::Zero();
            if (cell_inv.x() != 0.0) cell.x() = 1.0 / cell_inv.x();
            if (cell_inv.y() != 0.0) cell.y() = 1.0 / cell_inv.y();
            if (cell_inv.z() != 0.0) cell.z() = 1.0 / cell_inv.z();

            this->cell_inv = cell_inv;
        }

        template<typename E>
        auto wrap(E const& v) {
            VectorT in_box_ = v;
            in_box_.x() -= cell.x() * xmd::nearbyint(v.x() * cell_inv.x());
            in_box_.y() -= cell.y() * xmd::nearbyint(v.y() * cell_inv.y());
            in_box_.z() -= cell.z() * xmd::nearbyint(v.z() * cell_inv.z());
            return in_box_;
        }

        template<typename E>
        auto r_uv(E const& u, E const& v) {
            VectorT diff = v - u;
            return wrap(diff);
        }
    };
}
