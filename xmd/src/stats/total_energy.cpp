#include "stats/total_energy.h"

namespace xmd {
    void compute_total_energy::operator()() const {
        real K = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            K += (real)0.5 * mass[idx] * v3::norm_squared(v[idx]);
        }
        *E = *V + K;
    }

    void compute_total_energy::init_from_vm(vm &vm_inst) {
        v = vm_inst.find<vec3r_vector>("r").to_array();
        mass = vm_inst.find<vector<real>>("mass").to_array();
        num_particles = vm_inst.find<int>("num_particles");
        V = &vm_inst.find<real>("V");
        E = &vm_inst.find_or_emplace<real>("E", (real)0.0);
    }
}