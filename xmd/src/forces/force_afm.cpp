#include "forces/force_afm.h"

namespace xmd {
    void eval_force_afm_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto pulled_idx = bundles.pulled_idx[idx];
            F[pulled_idx] += bundles.pull_force[idx];
        }
    }

    void eval_force_afm_forces::init_from_vm(vm &vm_inst) {
        F = vm_inst.find<vec3r_vector>("F").to_array();
        bundles = vm_inst.find<force_afm_bundle_vector>("force_afm_bundles").to_span();
    }

    force_afm_bundle_vector::force_afm_bundle_vector(int n):
        pulled_idx(n), pull_force(n), size(n) {}

    force_afm_bundle_span force_afm_bundle_vector::to_span() {
        force_afm_bundle_span span;
        span.pulled_idx = pulled_idx.to_array();
        span.pull_force = pull_force.to_array();
        span.size = size;
        return span;
    };
}