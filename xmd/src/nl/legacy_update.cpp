#include "nl/legacy_update.h"
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>

namespace xmd::nl {
    void legacy_update::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        pad_factor = vm_inst.find_or_emplace<real>("pad_factor",
            params["neighbor list"]["pad factor"].as<quantity>());
        r = vm_inst.find<vector<vec3r>>("r").data();
        box = &vm_inst.find<xmd::box>("box");
        t = &vm_inst.find<float>("t");
        chain_idx = vm_inst.find<vector<int>>("chain_idx").data();
        seq_idx = vm_inst.find<vector<int>>("seq_idx").data();

        num_particles = vm_inst.find<int>("num_particles");

        data = &vm_inst.find_or<nl_data>("nl_data", [&]() -> auto& {
            auto& data_ = vm_inst.emplace<nl_data>("nl_data");
            data_.orig_r = vector<vec3r>(num_particles);
            return data_;
        });
        max_cutoff = &vm_inst.find_or_emplace<real>("max_cutoff");
        invalid = &vm_inst.find_or_emplace<bool>("invalid", true);
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