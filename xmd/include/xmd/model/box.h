#pragma once
#include <xmd/types/vec3.h>
#include <xmd/utils/math.h>

namespace xmd {
    class box {
    public:
        vec3r cell, cell_inv;

        inline box(): cell{vec3r::Zero()}, cell_inv{vec3r::Zero()} {};

        inline void set_cell(vec3r const& new_cell) {
            this->cell = new_cell;

            cell_inv = vec3r::Zero();
            if (cell.x() != 0.0)
                cell_inv.x() = (real)1.0 / cell.x();
            if (cell.y() != 0.0)
                cell_inv.y() = (real)1.0 / cell.y();
            if (cell.z() != 0.0)
                cell_inv.z() = (real)1.0 / cell.z();
        }

        inline void set_cell_inv(vec3r const& new_cell_inv) {
            this->cell_inv = new_cell_inv;

            cell = vec3r::Zero();
            if (cell_inv.x() != 0.0)
                cell.x() = (real)1.0 / cell_inv.x();
            if (cell_inv.y() != 0.0)
                cell.y() = (real)1.0 / cell_inv.y();
            if (cell_inv.z() != 0.0)
                cell.z() = (real)1.0 / cell_inv.z();
        }

        template<typename E>
        inline auto wrap(E const& v) const {
            vec3r in_box_ = v;
            in_box_.x() -= cell.x() * xmd::nearbyint(v.x() * cell_inv.x());
            in_box_.y() -= cell.y() * xmd::nearbyint(v.y() * cell_inv.y());
            in_box_.z() -= cell.z() * xmd::nearbyint(v.z() * cell_inv.z());
            return in_box_;
        }

        template<typename E>
        inline auto r_uv(E const& u, E const& v) const {
            vec3r diff = v - u;
            return wrap(diff);
        }

    private:
        friend class ::boost::serialization::access;

        template<class Archive>
            void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
                ar & cell;
                ar & cell_inv;
            }
    };
}
