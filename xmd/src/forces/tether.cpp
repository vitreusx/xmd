#include "forces/tether.h"
#include <xmd/model/model.h>
#include <unordered_map>
#include <xmd/utils/units.h>
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    void eval_tether_forces::operator()() const {
        for (int idx = 0; idx < tethers.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_tether_forces::omp_async() const {
#pragma omp for schedule(static) nowait
        for (int idx = 0; idx < tethers.size(); ++idx) {
            iter(idx);
        }
    }

    void eval_tether_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        H1 = ctx.persistent<real>("tether_H1",
            params["tether forces"]["H1"].as<quantity>());
        H2 = ctx.persistent<real>("tether_H2",
            params["tether forces"]["H2"].as<quantity>());
        def_length = ctx.persistent<real>("tether_def_length",
            params["tether forces"]["def_length"].as<quantity>());

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.per_thread().var<vector<vec3r>>("F").data();
        V = &ctx.per_thread().var<real>("V");

        tethers = ctx.persistent<vector<tether_pair>>("tethers",
            lazy([&]() -> auto {
                auto& xmd_model = ctx.var<model>("model");
                using res_map_t = std::unordered_map<xmd::model::residue*, int>;
                auto& res_map = ctx.var<res_map_t>("res_map");

                vector<tether_pair> tethers_;

                for (auto const& tether: xmd_model.tethers) {
                    auto i1 = res_map[tether.res1], i2 = res_map[tether.res2];
                    auto nat_dist = (real)tether.length.value_or(def_length);
                    tethers_.emplace_back(i1, i2, nat_dist);
                }

                return tethers_;
            })).view();
    }

    void eval_tether_forces::iter(int idx) const {
        auto tether = tethers[idx];
        auto i1 = tether.i1(), i2 = tether.i2();
        auto nat_dist = tether.nat_dist();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = r2 - r1;

        auto r12_n = norm(r12);
        auto [V_, dV_dr] = harmonic(H1, H2, nat_dist)(r12_n);

        auto r12_u = r12 / r12_n;
        *V += V_;
        F[i1] += dV_dr * r12_u;
        F[i2] -= dV_dr * r12_u;
    }
}