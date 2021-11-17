#pragma once

namespace xmd::qa {
    void precompute_nh::operator()() const {
        for (size_t i = 0 ; i < bundles.size(); ++i) {
            auto const& b = bundles[i];
            auto r1 = r[b.iprev], r2 = r[b.i], r3 = r[b.inext];
            auto v1 = r2 - r1, v2 = r3 - r2;
            n[b.i] = unit(v2 - v1);
            h[b.i] = unit(cross(v2, v1));
        }
    }

    void sift_formation_candidates::operator()() const {
        candidates.clear();

        for (size_t i = 0; i < pairs.size(); ++i) {
            auto const& p = pairs[i];
            auto r1 = r[p.i1], r2 = r[p.i2], r12 = box->uv(r1, r2);
            auto r12_rn = rnorm(r12);
            auto r12_u = r12 * r12_rn;

            auto n1 = n[p.i1], n2 = n[p.i2], h1 = h[p.i1], h2 = h[p.i2];

            candidate cand;

            auto back1 = (fabsf(dot(h1, r12_u)) > min_abs_cos_hr);
            auto back2 = (fabsf(dot(h2, r12_u)) > min_abs_cos_hr);
            auto bb_extra = (fabsf(dot(h1, h2)) > min_abs_cos_hh);
            auto side1 = (dot(n1, r12_u) < max_cos_nr);
            auto side2 = (-dot(n2, r12_u) < max_cos_nr);

            auto atype1 = atype[p.i1], atype2 = atype[p.i2];

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

            candidates.add(cand);
        }
    }

    void process_candidates::operator()() const {
        for (size_t i = 0; i < candidates.size(); ++i) {
            auto const& cand = candidates[i];
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
                contacts.add(cont);
            }
        }
    }

    void process_contacts::operator()() const {
        for (size_t i = 0; i < contacts.size(); ++i) {
            auto const& cont = contacts[i];
            auto r1 = r[cont.i1], r2 = r[cont.i2], r12 = box->uv(r1, r2);

            auto saturation = fminf(*t - cont.ref_time, cycle_time) * cycle_time_inv;
            if (cont.status == BREAKING)
                saturation = 1.0f - saturation;

            auto cont_depth = saturation * lj_depth[(short)cont.type];
            auto cont_r_min_inv = r_min_inv[(short)cont.type];



        }
    }
}