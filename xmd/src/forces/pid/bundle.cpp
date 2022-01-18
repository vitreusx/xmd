#include "forces/pid/bundle.h"
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::pid {

    void update_pid_bundles::operator()() const {
        bundles->clear();

        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size(); ++pair_idx) {
            auto nl_pair = nl->particle_pairs[pair_idx];
            auto i1 = nl_pair.i1(), i2 = nl_pair.i2();
            auto r1 = r[i1], r2 = r[i2];

            if (norm_inv(box->r_uv(r1, r2)) > min_norm_inv) {
                auto prev1 = prev[i1], next1 = next[i1];
                auto prev2 = prev[i2], next2 = next[i2];
                if (prev1 < 0 || next1 < 0 || prev2 < 0 || next2 < 0)
                    continue;

                auto atype1 = atype[i1], atype2 = atype[i2];
                int16_t type = (int16_t)atype1 *
                               (int16_t)amino_acid::NUM_AA + (int16_t)atype2;

                bundles->emplace_back(i1, i2, type);
            }
        }
    }
}