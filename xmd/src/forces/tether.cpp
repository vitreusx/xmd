#include "forces/tether.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/param_file.h>

namespace xmd {
    void eval_tether_forces::operator()() const {
//#pragma omp taskloop default(none) nogroup
        for (int idx = 0; idx < tethers.size; ++idx) {
            auto i1 = tethers.i1[idx], i2 = tethers.i2[idx];
            auto nat_dist = tethers.nat_dist[idx];

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = r2 - r1;

            auto r12_n = norm(r12);
            auto [V_, dV_dr] = harmonic(H1, H2, nat_dist)(r12_n);

            auto r12_u = r12 / r12_n;
//#pragma omp atomic update
            *V += V_;
            F[i1] += dV_dr * r12_u;
            F[i2] -= dV_dr * r12_u;
        }
    }

    void eval_tether_forces::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        H1 = vm_inst.find_or_emplace<real>("tether_H1",
            params["tether forces"]["H1"].as<quantity>());
        H2 = vm_inst.find_or_emplace<real>("tether_H2",
            params["tether forces"]["H2"].as<quantity>());
        def_length = vm_inst.find_or_emplace<real>("tether_def_length",
            params["tether forces"]["def_length"].as<quantity>());

        r = vm_inst.find<vec3r_vector>("r").to_array();
        F = vm_inst.find<vec3r_vector>("F").to_array();
        V = &vm_inst.find<real>("V");

        tethers = vm_inst.find_or<tether_pair_vector>("tethers",
            [&]() -> auto& {
                auto& xmd_model = vm_inst.find<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = vm_inst.find<res_map_t>("res_map");

                auto& tethers_ = vm_inst.emplace<tether_pair_vector>(
                    "tethers", xmd_model.tethers.size());
                int tether_idx = 0;
                for (auto const& tether: xmd_model.tethers) {
                    tethers_.i1[tether_idx] = res_map[tether.res1];
                    tethers_.i2[tether_idx] = res_map[tether.res2];
                    tethers_.nat_dist[tether_idx] = tether.length.value_or(def_length);
                    ++tether_idx;
                }

                return tethers_;
            }).to_span();
    }

    tether_pair_vector::tether_pair_vector(int n):
        i1{n}, i2{n}, nat_dist{n}, size{n} {}

    tether_pair_span tether_pair_vector::to_span() {
        tether_pair_span span;
        span.i1 = i1.to_array();
        span.i2 = i2.to_array();
        span.nat_dist = nat_dist.to_array();
        span.size = size;
        return span;
    }
}