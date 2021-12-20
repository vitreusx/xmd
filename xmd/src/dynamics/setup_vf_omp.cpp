#include "dynamics/setup_vf_omp.h"

namespace xmd {
    void setup_vf_omp::init_from_vm(vm& vm_inst) {
        num_particles = vm_inst.find<int>("num_particles");

        auto &shared_F = vm_inst.find<vec3r_vector>("F");
        vm_inst.emplace<vec3r_vector*>("shared_F_ptr", &shared_F);
        thread_F = vm_inst.emplace<vec3r_vector>("F", num_particles).to_array();

        auto &shared_V = vm_inst.find<real>("V");
        vm_inst.emplace<real*>("shared_V_ptr", &shared_V);
        thread_V = &vm_inst.emplace<real>("V");
    }

    void setup_vf_omp::operator()() const {
        *thread_V = 0.0f;
        for (int idx = 0; idx < num_particles; ++idx) {
            thread_F[idx] = vec3r::Zero();
        }
    }
}