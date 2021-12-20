#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/utils/geometry.h>
#include <xmd/vm/vm.h>
#include <xmd/model/box.h>

namespace xmd {
    enum lj_attr_pair_status: int8_t {
        FREE, FORMING_OR_FORMED, BREAKING
    };

    struct lj_attr_pairs_span {
        array<int> part_idx, wall_idx;
        array<lj_attr_pair_status> status;
        vec3r_array joint_r;
        array<real> ref_t;
        int size;
    };

    class lj_attr_pairs_vector {
    public:
        vector<int> part_idx, wall_idx;
        vector<lj_attr_pair_status> status;
        vec3r_vector joint_r;
        vector<real> ref_t;
        int size;

        explicit lj_attr_pairs_vector(int n = 0);
        lj_attr_pairs_span to_span();
    };

    class eval_lj_attr_wall_forces: public vm_aware {
    public:
        real wall_min_dist;
        real lj_depth;
        real cycle_time, cycle_time_inv;
        real breaking_factor, factor;

    public:
        vec3r_array r, F, wall_F;
        box<vec3r> *box;
        span<plane> walls;
        real *V, *t;
        int num_particles;
        lj_attr_pairs_span pairs;

        void init_from_vm(vm& vm_inst) override;

    public:
        void loop(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}