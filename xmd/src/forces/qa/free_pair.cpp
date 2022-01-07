#include "forces/qa/free_pair.h"
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::qa {

    void update_free_pairs::operator()() const {
        pairs->clear();

        auto cutoff = max_formation_min_dist;
        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size(); ++pair_idx) {
            auto nl_pair = nl->particle_pairs[pair_idx];
            auto i1 = nl_pair.i1(), i2 = nl_pair.i2();
            auto r1 = r[i1], r2 = r[i2];

            if (norm_inv(box->r_uv(r1, r2)) > min_norm_inv) {
                pairs->emplace(i1, i2);
            }
        }
    }

    void update_free_pairs::declare_vars(context& ctx) {
        r = ctx.var<vector<vec3r>>("r").data();
        box = &ctx.var<xmd::box>("box");
        nl = &ctx.var<nl::nl_data>("nl_data");
        pairs = &ctx.var<set<free_pair>>("qa_free_pairs");

        max_formation_min_dist = ctx.persistent<real>("max_formation_min_dist",
            lazy([&]() -> auto {
                real dist_ = 0.0;

                auto& variants = ctx.var<lj_variants>("lj_variants");
                dist_ = max(dist_, variants.bb.r_min());
                dist_ = max(dist_, variants.bs.r_min());
                dist_ = max(dist_, variants.sb.r_min());
                for (int ss_idx = 0; ss_idx < variants.ss.size(); ++ss_idx) {
                    auto ss = variants.ss[ss_idx];
                    dist_ = max(dist_, ss.r_max());
                }

                return dist_;
            }));

        auto& max_cutoff = ctx.var<real>("max_cutoff");
        max_cutoff = max(max_cutoff, max_formation_min_dist);
    }
}