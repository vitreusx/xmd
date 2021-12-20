#include "dynamics/reduce_vf_omp.h"

namespace xmd {
    void reduce_vf_omp::init_from_vm(vm& vm_inst) {
        num_particles = vm_inst.find<int>("num_particles");
        shared_F = vm_inst.find<vec3r_vector*>("shared_F_ptr")->to_array();
        thread_F = vm_inst.find<vec3r_vector>("F").to_array();
        shared_V = vm_inst.find<real*>("shared_V_ptr");
        thread_V = &vm_inst.find<real>("V");
    }

    void reduce_vf_omp::operator()() const {
#pragma omp atomic update
        *shared_V += *thread_V;

        for (int idx = 0; idx < num_particles; ++idx) {
            shared_F.atomic_at(idx) += thread_F[idx];
        }
    }
}