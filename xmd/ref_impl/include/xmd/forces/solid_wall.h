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
    public:
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
        void operator()() const;
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
        void operator()() const;
    };
}