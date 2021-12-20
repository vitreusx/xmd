#include "forces/velocity_afm.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_velocity_afm_forces::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            iter(idx);
        }
    }

    void eval_velocity_afm_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        afm_force.H1 = vm_inst.find_or_emplace<real>("vel_afm_H1",
            params["velocity AFM"]["H1"].as<quantity>());
        afm_force.H2 = vm_inst.find_or_emplace<real>("vel_afm_H2",
            params["velocity AFM"]["H2"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        t = &vm_inst.find<real>("t");

        bundles = vm_inst.find_or<velocity_afm_bundle_vector>("vel_afm_bundles",
            [&]() -> auto& {
                auto& bundles_ = vm_inst.emplace<velocity_afm_bundle_vector>(
                    "vel_afm_bundles");
                for (auto const& tip_node: params["velocity AFM"]["AFM tips"]) {
                    int bundle_idx = bundles_.push_back();
                    bundles_.pulled_idx[bundle_idx] = tip_node["residue idx"].as<int>();
                    bundles_.afm_orig[bundle_idx] = tip_node["origin"].as<vec3r>();
                    bundles_.afm_vel[bundle_idx] = tip_node["velocity"].as<vec3r>();
                }

                return bundles_;
            }).to_span();
    }

    void eval_velocity_afm_forces::iter(int idx) const {
        auto pulled_idx = bundles.pulled_idx[idx];
        auto r_ = r[pulled_idx];

        auto afm_orig = bundles.afm_orig[idx], afm_vel = bundles.afm_vel[idx];
        auto cur_afm_pos = afm_orig + *t * afm_vel;

        auto r_afm = cur_afm_pos - r_;
        auto r_afm_n = norm(r_afm), r_afm_rn = 1.0f / r_afm_n;
        auto r_afm_u = r_afm * r_afm_rn;

        auto [_, dV_dr] = afm_force(r_afm_n);
        F[pulled_idx] += dV_dr * r_afm_u;
    }

    void eval_velocity_afm_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < bundles.size; ++idx) {
            iter(idx);
        }
    }

    velocity_afm_bundle_vector::velocity_afm_bundle_vector(int n):
        pulled_idx(n), afm_orig(n), afm_vel(n), size(n) {}

    velocity_afm_bundle_span velocity_afm_bundle_vector::to_span() {
        velocity_afm_bundle_span span;
        span.pulled_idx = pulled_idx.to_array();
        span.afm_orig = afm_orig.to_array();
        span.afm_vel = afm_vel.to_array();
        span.size = size;
        return span;
    }

    int velocity_afm_bundle_vector::push_back() {
        pulled_idx.emplace_back();
        afm_orig.emplace_back();
        afm_vel.emplace_back();
        return size++;
    };
}