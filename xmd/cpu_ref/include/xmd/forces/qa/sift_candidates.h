#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact_type.h"
#include "free_pair.h"
#include "sync_data.h"
#include "candidate.h"

namespace xmd::qa {
    class sift_candidates {
    public:
        float min_abs_cos_hr, min_abs_cos_hh, max_cos_nr;
        float req_min_dist[contact_type::NUM_TYPES];
        polarization_type ptype[amino_acid::NUM_AA];

    public:
        vec3f_array r, n, h;
        box<vec3f> *box;
        amino_acid *atype;
        sync_data_array sync;

        free_pair_set free_pairs;
        candidate_list candidates;

    public:
        inline void operator()() {
            candidates.clear();

            for (int idx = 0; idx < free_pairs.extent(); ++idx) {
                if (!free_pairs.has_item(idx))
                    continue;

                auto i1 = free_pairs.i1[idx], i2 = free_pairs.i2[idx];

                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_rn = norm_inv(r12);
                auto r12_u = r12 * r12_rn;

                auto n1 = n[i1], n2 = n[i2], h1 = h[i1], h2 = h[i2];

                auto back1 = (abs(dot(h1, r12_u)) > min_abs_cos_hr);
                auto back2 = (abs(dot(h2, r12_u)) > min_abs_cos_hr);
                auto bb_extra = (abs(dot(h1, h2)) > min_abs_cos_hh);
                auto side1 = (dot(n1, r12_u) < max_cos_nr);
                auto side2 = (-dot(n2, r12_u) < max_cos_nr);

                auto atype1 = atype[i1], atype2 = atype[i2];

                contact_type type;
                if (back1 && back2 && bb_extra)
                    type = contact_type::BACK_BACK();
                else if (back1 && side2)
                    type = contact_type::BACK_SIDE();
                else if (side1 && back2)
                    type = contact_type::SIDE_BACK();
                else if (side1 && side2)
                    type = contact_type::SIDE_SIDE(atype1, atype2);

                if ((short)type == (short)contact_type::NONE())
                    continue;

                if (1.0f > r12_rn * req_min_dist[(short)type])
                    continue;

                auto ptype2 = ptype[atype2];
                sync_data sync_diff1;
                sync_diff1.back += back1;
                sync_diff1.side_all += side1;
                sync_diff1.side_polar += side1 && (ptype2 == POLAR);
                sync_diff1.side_hydrophobic += side1 && (ptype2 == HYDROPHOBIC);

                auto sync1_after_formation = sync[i1] - sync_diff1;
                if (!sync1_after_formation.is_valid())
                    continue;

                auto ptype1 = ptype[atype1];
                sync_data sync_diff2;
                sync_diff2.back += back2;
                sync_diff2.side_all += side2;
                sync_diff2.side_polar += side2 && (ptype1 == POLAR);
                sync_diff2.side_hydrophobic += side2 && (ptype1 == HYDROPHOBIC);

                auto sync2_after_formation = sync[i2] - sync_diff2;
                if (!sync2_after_formation.is_valid())
                    continue;

                int slot_idx = candidates.add();
                candidates.i1[slot_idx] = i1;
                candidates.i2[slot_idx] = i2;
                candidates.type[slot_idx] = type;
                candidates.sync_diff1[slot_idx] = sync_diff1;
                candidates.sync_diff2[slot_idx] = sync_diff2;
            }
        }
    };
}