#include "dynamics/setup_vf_omp.h"

namespace xmd {
    void setup_vf_omp::init_from_vm(vm& vm_inst) {
        num_particles = vm_inst.find<int>("num_particles");

        auto &shared_F = vm_inst.find<vector<vec3r>>("F");
        vm_inst.emplace<vector<vec3r>*>("shared_F_ptr", &shared_F);
        thread_F = vm_inst.emplace<vector<vec3r>>("F", num_particles).data();

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