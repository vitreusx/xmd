#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/utils/geometry.h>
#include <xmd/vm/vm.h>
#include <xmd/model/box.h>

namespace xmd {
    class eval_solid_wall_forces: public vm_aware {
    public:
        real eps, cutoff;

    public:
        vec3r_array r, F, wall_F;
        box<vec3r> *box;
        span<plane> walls;
        real *V;
        int num_particles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}