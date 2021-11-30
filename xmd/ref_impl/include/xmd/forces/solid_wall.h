#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/types/geometry.h>

namespace xmd {
    struct solid_wall_pair_span {
        array<int> wall_idx, part_idx;
        int size;
    };

    class solid_wall_pair_vector {
        vector<int> wall_idx, part_idx;
        int size;

        inline int push_back() {
            wall_idx.push_back();
            part_idx.push_back();
            return size++;
        }

        inline void clear() {
            wall_idx.clear();
            part_idx.clear();
            size = 0;
        }

        inline auto to_span() const {
            solid_wall_pair_span s;
            s.wall_idx = wall_idx.data();
            s.part_idx = part_idx.data();
            s.size = size;
            return s;
        }
    };

    class update_solid_wall_pairs {
    public:
        float cutoff;

    public:
        vec3f_array r;
        int num_particles;
        span<planef> walls;
        solid_wall_pair_vector *pairs;

    public:
        inline void operator()() const {
            pairs->clear();
            for (int part_idx = 0; part_idx < num_particles; ++part_idx) {
                auto r_ = r[part_idx];
                for (int wall_idx = 0; wall_idx < walls.size(); ++wall_idx) {
                    if (abs(v3::signed_dist(r, walls[wall_idx])) < cutoff) {
                        auto pair_idx = pairs->push_back();
                        pairs->part_idx[pair_idx] = part_idx;
                        pairs->wall_idx[pair_idx] = wall_idx;
                    }
                }
            }
        }
    };

    class eval_solid_wall_forces {
    public:
        float eps;

    public:
        vec3f_array r, F, wall_F;
        solid_wall_pair_span pairs;
        span<planef> walls;
        float *V;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < pairs.size; ++idx) {
                auto part_idx = pairs.part_idx[idx],
                    wall_idx = pairs.wall_idx[idx];

                auto r_ = r[part_idx];
                auto wall = walls[wall_idx];
                auto d = v3::signed_dist(r_, wall);

                auto V_ = eps / (9.0f * ipow<9>(d)), dV_dd = V_ * (-9.0f * d);
                *V += V_;
                F[part_idx] -= dV_dd * wall.normal;
                wall_F[wall_idx] += dV_dd * wall.normal;
            }
        }
    };
}