#include "nl/legacy_update.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>

namespace xmd::nl {
    void legacy_update::operator()() const {
        auto pad = pad_factor * *max_cutoff;
        auto req_r = *max_cutoff + pad;

        data->native_contacts.clear();
        data->native_ssbonds.clear();
        data->non_native.clear();

        int nat_cont_idx = 0, nat_ss_idx = 0;

        for (int i1 = 0; i1 < num_particles; ++i1) {
            auto r1 = r[i1];
            auto chain1 = chain_idx[i1], seq1 = seq_idx[i1];

            for (int i2 = i1 + 1; i2 < num_particles; ++i2) {
                auto r2 = r[i2];
                auto chain2 = chain_idx[i2], seq2 = seq_idx[i2];

                auto diff = seq1 > seq2 ? seq1 - seq2 : seq2 - seq1;
                if (chain1 == chain2 && diff < 3)
                    continue;

                auto orig_dist = norm(box->r_uv(r1, r2));
                if (orig_dist < req_r) {
                    bool non_native = true;
                    auto p = nl_pair(i1, i2, orig_dist);

                    while (nat_cont_idx < all_nat_cont.size()) {
                        auto cur_nat_cont = all_nat_cont[nat_cont_idx];

                        if (cur_nat_cont == p) {
                            data->native_contacts.push_back(p);
                            non_native = false;
                        }

                        if (cur_nat_cont >= p)
                            break;
                    }

                    while (nat_ss_idx < all_nat_ss.size()) {
                        auto cur_nat_ss = all_nat_ss[nat_ss_idx];

                        if (cur_nat_ss == p) {
                            data->native_ssbonds.push_back(p);
                            non_native = false;
                        }

                        if (cur_nat_ss >= p)
                            break;
                    }

                    if (non_native) {
                        data->native_contacts.push_back(p);
                    }
                }
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