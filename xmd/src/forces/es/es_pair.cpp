#include "forces/es/es_pair.h"
#include <xmd/params/yaml_fs_node.h>
#include <array>

namespace xmd {
    void update_es_base::operator()() const {
        pairs->clear();

        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size(); ++pair_idx) {
            auto pair = nl->particle_pairs[pair_idx];
            auto i1 = pair.i1(), i2 = pair.i2();
            auto r1 = r[i1], r2 = r[i2];

            if (norm_inv(box->r_uv(r1, r2)) > min_norm_inv) {
                auto q1_x_q2 = q[(short)atype[i1]] * q[(short)atype[i2]];
                if (q1_x_q2 != 0.0) {
                    pairs->emplace_back(i1, i2, q1_x_q2);
                }
            }
        }
    }

    void update_es_base::declare_vars(context& ctx) {
        using stored_q_t = std::array<real, amino_acid::NUM_AA>;
        auto& stored_q = ctx.persistent<stored_q_t>("stored_q", lazy([&]() -> auto {
            stored_q_t stored_q_;

            auto& aa_data_ = ctx.var<amino_acid_data>(
                "amino_acid_data");
            for (auto const& aa: amino_acid::all()) {
                stored_q_[(short)aa] = aa_data_[aa].charge;
            }

            return stored_q_;
        }));

        for (auto const& aa: amino_acid::all())
            q[(short)aa] = stored_q[(short)aa];

        r = ctx.var<vector<vec3r>>("r").data();
        box = &ctx.var<xmd::box>("box");
        nl = &ctx.var<nl::nl_data>("nl_data");
        pairs = &ctx.var<vector<es_pair>>("es_pairs");
        atype = ctx.var<vector<amino_acid>>("atype").data();
    }
}