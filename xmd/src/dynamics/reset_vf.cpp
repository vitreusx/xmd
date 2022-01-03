#include "dynamics/reset_vf.h"

namespace xmd {
    void reset_vf::operator()() const {
        *V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3r::Zero();
        }
    }

    void reset_vf::init_from_vm(vm &vm_inst) {
        F = vm_inst.find<vector<vec3r>>("F").data();
        V = &vm_inst.find<real>("V");
        num_particles = vm_inst.find<int>("num_particles");
    }

    void reset_vf::omp_async() const {
        *V = 0.0f;

#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3r::Zero();
        }
    }
}