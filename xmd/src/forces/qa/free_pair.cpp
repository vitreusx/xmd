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
}