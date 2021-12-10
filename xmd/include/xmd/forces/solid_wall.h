#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/utils/geometry.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct solid_wall_pair_span {
        array<int> wall_idx, part_idx;
        int size;
    };

    class solid_wall_pair_vector {
    public:
        vector<int> wall_idx, part_idx;
        int size;

        explicit solid_wall_pair_vector(int n = 0);

        int push_back();
        void clear();
        solid_wall_pair_span to_span();
    };

    class update_solid_wall_pairs: public vm_aware {
    public:
        real cutoff;

    public:
        vec3r_array r;
        int num_particles;
        span<planef> walls;
        solid_wall_pair_vector *pairs;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };

    class eval_solid_wall_forces: public vm_aware {
    public:
        real eps;

    public:
        vec3r_array r, F, wall_F;
        solid_wall_pair_span pairs;
        span<planef> walls;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}