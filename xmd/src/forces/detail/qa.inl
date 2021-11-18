#pragma once

namespace xmd::qa {
    void precompute_nh::operator()() {
        for (size_t i = 0 ; i < bundles.size(); ++i) {
            const_ref<nh_bundle> b = bundles[i];
            const_ref<vec3f> r1 = r[b.iprev], r2 = r[b.i], r3 = r[b.inext];
            auto v1 = r2 - r1, v2 = r3 - r2;
            n[b.i] = unit(v2 - v1);
            h[b.i] = unit(cross(v2, v1));
        }
    }

    void sift_formation_candidates::operator()() {
        candidates.clear();

        for (size_t i = 0; i < pairs.size(); ++i) {
            const_ref<unformed_pair> p = pairs[i];
            const_ref<vec3f> r1 = r[p.i1], r2 = r[p.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_rn = rnorm(r12);
            auto r12_u = r12 * r12_rn;

            const_ref<vec3f> n1 = n[p.i1], n2 = n[p.i2], h1 = h[p.i1], h2 = h[p.i2];

            candidate cand;

            auto back1 = (fabsf(dot(h1, r12_u)) > min_abs_cos_hr);
            auto back2 = (fabsf(dot(h2, r12_u)) > min_abs_cos_hr);
            auto bb_extra = (fabsf(dot(h1, h2)) > min_abs_cos_hh);
            auto side1 = (dot(n1, r12_u) < max_cos_nr);
            auto side2 = (-dot(n2, r12_u) < max_cos_nr);

            const_ref<amino_acid> atype1 = atype[p.i1], atype2 = atype[p.i2];

            if (back1 && back2 && bb_extra)
                cand.type = contact_type::BB();
            else if (back1 && side2)
                cand.type = contact_type::BS();
            else if (side1 && back2)
                cand.type = contact_type::SB();
            else if (side1 && side2)
                cand.type = contact_type::SS(atype1, atype2);

            if (!cand.type)
                return;

            if (1.0f > r12_rn * r_min[(short)cand.type])
                return;

            auto ptype2 = ptype[atype2];
            cand.sync_diff1.back += back1;
            cand.sync_diff1.side_all += side1;
            cand.sync_diff1.side_h += side1 && (ptype2 == HYDROPHOBIC);
            cand.sync_diff1.side_p += side1 && (ptype2 == POLAR);
            if (!(sync_ns[p.i1] - cand.sync_diff1))
                return;

            auto ptype1 = ptype[atype1];
            cand.sync_diff2.back += back2;
            cand.sync_diff2.side_all += side2;
            cand.sync_diff2.side_h += side2 && (ptype1 == HYDROPHOBIC);
            cand.sync_diff2.side_p += side2 && (ptype1 == POLAR);
            if (!(sync_ns[p.i2] - cand.sync_diff2))
                return;

            candidates.push_back(cand);
        }
    }

    void process_candidates::operator()() {
        for (size_t i = 0; i < candidates.size(); ++i) {
            const_ref<candidate> cand = candidates[i];
            auto new_sync1 = sync_ns[cand.i1] - cand.sync_diff1;
            auto new_sync2 = sync_ns[cand.i2] - cand.sync_diff2;

            if (new_sync1 && new_sync2) {
                contact cont;
                cont.i1 = cand.i1;
                cont.i2 = cand.i2;
                cont.type = cand.type;
                cont.sync_diff1 = cand.sync_diff1;
                cont.sync_diff2 = cand.sync_diff2;
                cont.ref_time = *t;
                cont.status = FORMING_OR_FORMED;
                contacts.push_back(cont);
            }
        }
    }

    void process_contacts::operator()() {
        marked_for_erasure.clear();

        for (size_t i = 0; i < contacts.size(); ++i) {
            ref<contact> cont = contacts[i];
            ref<vec3f> r1 = r[cont.i1], r2 = r[cont.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_rn = rnorm(r12);
            auto r12_u = r12 * r12_rn;

            auto saturation = fminf(*t - cont.ref_time, cycle_time) * cycle_time_inv;
            if (cont.status == BREAKING)
                saturation = 1.0f - saturation;

            auto lj_force = lj_forces[(short)cont.type];
            auto [Vij, dVij_dr] = lj_force(r12_rn);
            *V += saturation * Vij;
            auto f = saturation * dVij_dr * r12_u;
            F[cont.i1] -= f;
            F[cont.i2] += f;

            if (cont.status == FORMING_OR_FORMED && saturation == 1.0f) {
                if (breaking_factor * powf(2.0f, -1.0f/6.0f) * lj_force.r_min * r12_rn < 1.0f) {
                    cont.status = BREAKING;
                    cont.ref_time = *t;
                }
            }
            else if (cont.status == BREAKING && saturation == 0.0f) {
                marked_for_erasure.push_back(i);
            }
        }

        for (ssize_t i = (ssize_t)marked_for_erasure.size()-1; i >= 0; --i) {
            contacts.erase(marked_for_erasure[i]);
        }
    }
}