#include "forces/solid_wall.h"

namespace xmd {

    void update_solid_wall_pairs::operator()() const {
        pairs->clear();
        for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
            auto r_ = r[part_idx];
            for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                if (abs(signed_dist(r_, walls[wall_idx])) < cutoff) {
                    auto pair_idx = pairs->push_back();
                    pairs->part_idx[pair_idx] = part_idx;
                    pairs->wall_idx[pair_idx] = wall_idx;
                }
            }
        }
    }

    void eval_solid_wall_forces::operator()() const {
        for (int idx = 0; idx < pairs.size; ++idx) {
            auto part_idx = pairs.part_idx[idx],
                wall_idx = pairs.wall_idx[idx];

            auto r_ = r[part_idx];
            auto wall = walls[wall_idx];
            auto d = v3::signed_dist(r_, wall);

            auto V_ = eps / (9.0f * ipow<9>(d)), dV_dd = V_ * (-9.0f * d);
            *V += V_;
            F[part_idx] += dV_dd * wall.normal;
            wall_F[wall_idx] -= dV_dd * wall.normal;
        }
    }
}