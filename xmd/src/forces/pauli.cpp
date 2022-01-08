#include "forces/pauli.h"
#include <xmd/utils/units.h>
#include <xmd/params/yaml_fs_node.h>
#include <xmd/forces/primitives/shifted_lj.h>
#include <iostream>

namespace xmd {

    void update_pauli_pairs::operator()() const {
        pairs->clear();

        auto cutoff = r_excl;
        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size(); ++pair_idx) {
            auto pair = nl->particle_pairs[pair_idx];
            auto i1 = pair.i1(), i2 = pair.i2();

            auto r1 = r[i1], r2 = r[i2];
            if (norm_inv(box->r_uv(r1, r2)) > min_norm_inv) {
                pairs->emplace_back(i1, i2);
            }
        }
    }

    void update_pauli_pairs::declare_vars(context& ctx) {
        r = ctx.var<vector<vec3r>>("r").data();
        box = &ctx.var<xmd::box>("box");
        nl = &ctx.var<nl::nl_data>("nl_data");
        pairs = &ctx.var<vector<pauli_pair>>("pauli_pairs");

        auto& max_cutoff = ctx.var<real>("max_cutoff");
        r_excl = ctx.var<real>("pauli_r_excl");
        max_cutoff = max(max_cutoff, r_excl);
    }

    void eval_pauli_exclusion_forces::operator()() const {
        for (int idx = 0; idx < pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_pauli_exclusion_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        r_excl = ctx.persistent<real>("pauli_r_excl",
            params["Pauli exclusion"]["r_excl"].as<quantity>());
        depth = ctx.persistent<real>("pauli_depth",
            params["Pauli exclusion"]["depth"].as<quantity>());

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.per_thread().var<vector<vec3r>>("F").data();
        V = &ctx.per_thread().var<real>("V");
        box = &ctx.var<xmd::box>("box");
        pairs = &ctx.persistent<vector<pauli_pair>>("pauli_pairs");
    }

    void eval_pauli_exclusion_forces::iter(int idx) const {
        auto pair = pairs->at(idx);
        auto i1 = pair.i1(), i2 = pair.i2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_rn = norm_inv(r12);

        if (1.0f < r12_rn * r_excl) {
            auto r12_u = r12 * r12_rn;
            auto [V_, dV_dr] = shifted_lj(depth, r_excl)(r12_rn);

            *V += V_;
            F[i1] += r12_u * dV_dr;
            F[i2] -= r12_u * dV_dr;
        }
    }

    void eval_pauli_exclusion_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < pairs->size(); ++idx) {
            iter(idx);
        }
    }
}