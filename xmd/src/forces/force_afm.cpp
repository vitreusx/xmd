#include "forces/force_afm.h"
#include <xmd/params/param_file.h>

namespace xmd {
    void eval_force_afm_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            iter(idx);
        }
    }

    void eval_force_afm_forces::init_from_vm(vm &vm_inst) {
        F = vm_inst.find<vec3r_vector>("F").to_array();

        bundles = vm_inst.find_or<force_afm_bundle_vector>("force_afm_bundles",
            [&]() -> auto& {
                auto& bundles_ = vm_inst.emplace<force_afm_bundle_vector>(
                    "vel_afm_bundles");

                auto& params = vm_inst.find<param_file>("params");
                for (auto const& tip_node: params["velocity AFM"]["AFM tips"]) {
                    int bundle_idx = bundles_.push_back();
                    bundles_.pulled_idx[bundle_idx] = tip_node["residue idx"].as<int>();
                    bundles_.pull_force[bundle_idx] = tip_node["pull force"].as<vec3r>();
                }

                return bundles_;
            }).to_span();
    }

    void eval_force_afm_forces::iter(int idx) const {
        auto pulled_idx = bundles.pulled_idx[idx];
        F[pulled_idx] += bundles.pull_force[idx];
    }

    void eval_force_afm_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < bundles.size; ++idx) {
            iter(idx);
        }
    }

    force_afm_bundle_vector::force_afm_bundle_vector(int n):
        pulled_idx(n), pull_force(n), size(n) {}

    force_afm_bundle_span force_afm_bundle_vector::to_span() {
        force_afm_bundle_span span;
        span.pulled_idx = pulled_idx.to_array();
        span.pull_force = pull_force.to_array();
        span.size = size;
        return span;
    }

    int force_afm_bundle_vector::push_back() {
        pulled_idx.emplace_back();
        pull_force.emplace_back();
        return size++;
    };
}