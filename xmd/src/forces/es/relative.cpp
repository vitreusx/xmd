#include "forces/es/relative.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd {

    void eval_relative_es_forces::operator()() const {
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void eval_relative_es_forces::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        auto const& rel_es_params = params["relative ES"];

        factor = ctx.persistent<real>("relative_ES_A",
            rel_es_params["factor"].as<quantity>());
        V_factor = 1.0f / (4.0f * (real)M_PI * factor);

        auto screening_dist = ctx.persistent<real>("screening_dist",
            rel_es_params["screening distance"].as<quantity>());
        screen_dist_inv = (real)1.0 / screening_dist;

        r = ctx.var<vector<vec3r>>("r").data();
        F = ctx.var<vector<vec3r>>("F").data();
        V = &ctx.var<real>("V");
        box = &ctx.var<xmd::box>("box");
        es_pairs = &ctx.persistent<vector<es_pair>>("es_pairs");
    }

    void eval_relative_es_forces::iter(int idx) const {
        auto es = es_pairs->at(idx);
        auto i1 = es.i1(), i2 = es.i2();
        auto q1_x_q2 = es.q1_x_q2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
        auto r12_n = norm(r12), r12_rn = 1.0f / r12_n;
        auto r12_u = r12 * r12_rn;

        auto Vij = V_factor * q1_x_q2 * exp(-r12_n * screen_dist_inv) * r12_rn * r12_rn;
        auto dVij_dr = -Vij*(screen_dist_inv+2.0f*r12_rn);

        *V += Vij;

        auto f = r12_u * dVij_dr;
        F[i1] += f;
        F[i2] -= f;

    }

    void eval_relative_es_forces::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < es_pairs->size(); ++idx) {
            iter(idx);
        }
    }

    void update_relative_es::declare_vars(context& ctx) {
        update_es_base::declare_vars(ctx);

        auto& params = ctx.var<yaml_fs_node>("params");
        auto const& rel_es_params = params["relative ES"];
        auto screening_dist = ctx.persistent<real>("screening_dist",
            rel_es_params["screening distance"].as<quantity>());
        cutoff = 2.0 * screening_dist;

        auto& max_cutoff = ctx.var<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    void update_relative_es::operator()() const {
        update_es_base::operator()();
    }
}