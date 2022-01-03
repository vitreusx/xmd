#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        real K_ = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            K_ += (real)0.5 * mass[idx] * norm_squared(v[idx]);
        }
        *K = K_;
        *E = *V + K_;
    }

    void compute_total_energy::init_from_vm(vm &vm_inst) {
        v = vm_inst.find<vector<vec3r>>("r").data();
        mass = vm_inst.find<vector<real>>("mass").data();
        num_particles = vm_inst.find<int>("num_particles");
        V = &vm_inst.find<real>("V");
        K = &vm_inst.find_or_emplace<real>("K", (real)0.0);
        E = &vm_inst.find_or_emplace<real>("E", (real)0.0);
    }
}