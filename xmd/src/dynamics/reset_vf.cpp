#include "dynamics/reset_vf.h"

namespace xmd {
    void reset_vf::operator()() const {
        *V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            F[idx] = vec3r::Zero();
        }
    }

    void reset_vf::init_from_vm(vm &vm_inst) {
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");
        num_particles = vm_inst.find<int>("num_particles");
    }
}