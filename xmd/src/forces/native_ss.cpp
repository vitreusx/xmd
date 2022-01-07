#include "forces/native_ss.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    void update_nat_ssbonds::operator()() const {
        ssbonds->clear();

        for (int idx = 0; idx < all_ssbonds->size(); ++idx) {
            auto ssbond = all_ssbonds->at(idx);
            auto i1 = ssbond.i1(), i2 = ssbond.i2();
            auto r1 = r[i1], r2 = r[i2];
            if (norm(box->r_uv(r1, r2)) < cutoff + nl->orig_pad) {
                ssbonds->emplace_back(i1, i2);
            }
        }
    }

    void update_nat_ssbonds::declare_vars(context& ctx) {
        r = ctx.var<vector<vec3r>>("r").data();
        box = &ctx.var<xmd::box>("box");
        nl = &ctx.var<nl::nl_data>("nl_data");
        all_ssbonds = &ctx.persistent<vector<nat_ss>>("all_ssbonds",
            lazy([&]() -> auto {
                auto& xmd_model = ctx.var<xmd::model>("model");
                using res_map_t = std::unordered_map<
                    xmd::model::residue*, int>;
                auto& res_map = ctx.var<res_map_t>("res_map");

                vector<nat_ss> all_ssbonds_;

                for (auto const& cont: xmd_model.contacts) {
                    if (cont.type == model::NAT_SS) {
                        auto i1 = res_map[cont.res1], i2 = res_map[cont.res2];
                        all_ssbonds_.emplace_back(i1, i2);
                    }
                }

                return all_ssbonds_;
            }));

        ssbonds = &ctx.var<vector<nat_ss>>("ssbonds");

        auto& nat_r = ctx.var<real>("nat_ss_r");
        cutoff = harmonic::cutoff(nat_r);

        auto& max_cutoff = ctx.var<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_nat_ssbond_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        H1 = ctx.persistent<real>("nat_ss_H1",
            params["native ssbonds"]["H1"].as<quantity>());
        nat_r = ctx.persistent<real>("nat_ss_r",
            params["native ssbonds"]["equilibrium dist"].as<quantity>());

        box = &ctx.var<xmd::box>("box");
        ssbonds = &ctx.persistent<vector<nat_ss>>("ssbonds");
        r = ctx.var<vector<vec3r>>("r").data();
        V = &ctx.var<real>("V");
    }

    void eval_nat_ssbond_forces::iter(int idx) const {
        auto ssbond = ssbonds->at(idx);
        auto cys_i1 = ssbond.i1(), cys_i2 = ssbond.i2();
        auto r1 = r[cys_i1], r2 = r[cys_i2];
        auto r12 = box->r_uv(r1, r2);

        auto r12_n = norm(r12);
        auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

        auto r12_u = r12 / r12_n;
//#pragma omp atomic update
        *V += V_;
        F[cys_i1] += dV_dr * r12_u;
        F[cys_i2] -= dV_dr * r12_u;
    }

    void eval_nat_ssbond_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < ssbonds->size(); ++idx) {
            iter(idx);
        }
    }
}