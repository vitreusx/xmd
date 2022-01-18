#include "forces/walls/solid.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_solid_wall_forces::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < num_particles; ++idx)
            iter(idx);
    }

    void eval_solid_wall_forces::iter(int idx) const {
        auto part_r = box->wrap(r[idx]);
        for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
            auto wall = walls[wall_idx];
            auto x = signed_dist(part_r, wall);
            if (x < cutoff) {
                auto V_ = eps / ((real)9.0 * ipow<9>(x));
                auto dV_dx = V_ * ((real)(-9.0) * x);
                *V += V_;
                F[idx] += dV_dx * wall.normal();
                wall_F[wall_idx] -= dV_dx * wall.normal();
            }
        }
    }
}