#include "forces/dynss/sift_candidates.h"

namespace xmd::dynss {

    void sift_candidates::operator()() const {
        candidates->clear();

        for (int idx = 0; idx < free_cys_pairs->size(); ++idx) {
            if (!free_cys_pairs->has_item(idx))
                continue;

            auto i1 = free_cys_pairs->i1[idx], i2 = free_cys_pairs->i2[idx];
            if (has_formed_ssbond[i1] || has_formed_ssbond[i2])
                continue;

            auto r1 = r[i1], r2 = r[i2];
            auto r12 = box->ray(r1, r2);
            auto r12_rn = norm_inv(r12);
            auto r12_u = r12 * r12_rn;

            auto n1 = n[i1], n2 = n[i2];

            auto side1 = (dot(n1, r12_u) < max_cos_nr);
            auto side2 = (-dot(n2, r12_u) < max_cos_nr);
            if (!side1 || !side2)
                continue;

            if (1.0f > r12_rn * req_min_dist)
                continue;

            auto sync1_after_formation = sync[i1] - def_sync_diff;
            if (!sync1_after_formation.is_valid())
                continue;

            auto sync2_after_formation = sync[i2] - def_sync_diff;
            if (!sync2_after_formation.is_valid())
                continue;

            int slot_idx = candidates->add();
            candidates->i1[slot_idx] = i1;
            candidates->i2[slot_idx] = i2;
        }
    }
}