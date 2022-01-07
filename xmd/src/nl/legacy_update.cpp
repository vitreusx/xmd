#include "nl/legacy_update.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd::nl {
    void legacy_update::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        pad_factor = ctx.persistent<real>("pad_factor",
            params["neighbor list"]["pad factor"].as<quantity>());
        r = ctx.var<vector<vec3r>>("r").data();
        box = &ctx.var<xmd::box>("box");
        t = &ctx.var<float>("t");
        chain_idx = ctx.var<vector<int>>("chain_idx").data();
        seq_idx = ctx.var<vector<int>>("seq_idx").data();

        num_particles = ctx.var<int>("num_particles");

        data = &ctx.ephemeral<nl_data>("nl_data");
        data->orig_r = vector<vec3r>(num_particles);

        max_cutoff = &ctx.persistent<real>("max_cutoff");
        invalid = &ctx.ephemeral<bool>("invalid", true);
    }

    void legacy_update::operator()() const {
        auto pad = pad_factor * *max_cutoff;
        auto req_r = *max_cutoff + pad;

        data->particle_pairs.clear();
        for (int i1 = 0; i1 < num_particles; ++i1) {
            auto r1 = r[i1];
            auto chain1 = chain_idx[i1], seq1 = seq_idx[i1];

            for (int i2 = i1 + 1; i2 < num_particles; ++i2) {
                auto r2 = r[i2];
                auto chain2 = chain_idx[i2], seq2 = seq_idx[i2];

                auto diff = seq1 > seq2 ? seq1 - seq2 : seq2 - seq1;
                if (chain1 == chain2 && diff < 3)
                    continue;

                if (norm(box->r_uv(r1, r2)) < req_r)
                    data->particle_pairs.emplace_back(i1, i2, false);
            }
        }

        data->orig_pad = pad;
        for (int idx = 0; idx < num_particles; ++idx)
            data->orig_r[idx] = r[idx];
        data->orig_box = *box;
        data->ref_t = *t;
        *invalid = false;
    }
}