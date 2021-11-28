#pragma once
#include <xmd/forces/qa/contact_type.h>
#include <xmd/forces/qa/free_pair.h>
#include <xmd/forces/qa/sync_data.h>
#include <xmd/types/vec3.h>
#include "candidate.h"

namespace xmd::dynss {
    class sift_candidates {
    public:
        float max_cos_nr;
        float req_min_dist;
        polarization_type cys_ptype;
        qa::sync_data def_sync_diff;

    public:
        vec3f_array r, n;
        int8_t *has_formed_ssbond;
        box<vec3f> *box;
        qa::sync_data_array sync;

        qa::free_pair_list free_cys_pairs;
        candidate_list candidates;

    public:
        inline void operator()() {
            candidates.clear();

            for (int idx = 0; idx < free_cys_pairs.extent(); ++idx) {
                if (!free_cys_pairs.has_item(idx))
                    continue;

                auto i1 = free_cys_pairs.i1[idx], i2 = free_cys_pairs.i2[idx];
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

                int slot_idx = candidates.add();
                candidates.i1[slot_idx] = i1;
                candidates.i2[slot_idx] = i2;
            }
        }
    };
}