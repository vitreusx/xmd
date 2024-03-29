#pragma once


#include <xmd/utils/geometry.h>

#include <xmd/model/box.h>

namespace xmd {
    enum lj_attr_pair_status : int8_t {
        FREE, FORMING_OR_FORMED, BREAKING
    };
}

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() lj_attr_pair
#define FIELDS() int,part_idx,int,wall_idx,xmd::lj_attr_pair_status,status,\
xmd::vec3r,joint_r,xmd::real,ref_t

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_lj_attr_wall_forces {
    public:
        real wall_min_dist;
        real lj_depth;
        real cycle_time, cycle_time_inv;
        real breaking_factor, factor;

    public:
        const_array<vec3r> r;
        array<vec3r> F, wall_F;
        box const *box;
        span<plane> walls;
        real *V, *t;
        int num_particles;
        span<lj_attr_pair> pairs;



    public:
        void loop(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}