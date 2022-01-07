#include "forces/qa/sift_candidates.h"
#include <xmd/params/yaml_fs_node.h>
#include <xmd/utils/units.h>
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::qa {

    void sift_candidates::operator()() const {
        candidates->clear();
        for (int idx = 0; idx < free_pairs->extent(); ++idx) {
            iter(idx);
        }
    }

    void sift_candidates::declare_vars(context& ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        auto const& sift_params = params["quasi-adiabatic"];

        min_abs_cos_hr = sift_params["min |cos(h, r)|"].as<quantity>();
        min_abs_cos_hh = sift_params["min |cos(h, h)| for bb"].as<quantity>();
        max_cos_nr = sift_params["max cos(n, r)"].as<quantity>();

        auto& ljs = ctx.var<lj_variants>("lj_variants");

        req_min_dist[(short)contact_type::BACK_BACK()] = ljs.bb.r_min();
        req_min_dist[(short)contact_type::BACK_SIDE()] = ljs.bs.r_min();
        req_min_dist[(short)contact_type::SIDE_BACK()] = ljs.sb.r_min();
        for (auto const& aa1: amino_acid::all()) {
            for (auto const& aa2: amino_acid::all()) {
                int ss_idx = (int)aa1 * amino_acid::NUM_AA + (int)aa2;
                req_min_dist[(short)contact_type::SIDE_SIDE(aa1, aa2)] =
                    ljs.ss[ss_idx].r_max();
            }
        }

        auto& aa_data_ = ctx.var<amino_acid_data>(
            "amino_acid_data");
        for (auto const& aa: amino_acid::all()) {
            ptype[(int)aa] = aa_data_[aa].polarization;
        }

        candidates = &ctx.var<set<candidate>>("qa_candidates");
        atype = ctx.var<vector<amino_acid>>("atype").data();
        box = &ctx.var<xmd::box>("box");
        r = ctx.var<vector<vec3r>>("r").data();
        free_pairs = &ctx.var<set<free_pair>>("qa_free_pairs");
        sync = ctx.var<vector<sync_data>>("sync").data();
        n = ctx.var<vector<vec3r>>("qa_n").data();
        h = ctx.var<vector<vec3r>>("qa_h").data();
    }

    void sift_candidates::iter(int idx) const {
        auto node = free_pairs->at(idx);
        if (node.vacant()) return;
        auto pair = node.value();

        auto i1 = pair.i1(), i2 = pair.i2();

        auto r1 = r[i1], r2 = r[i2];
        auto r12 = box->r_uv(r1, r2);
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
            return;

        if (1.0f > r12_rn * req_min_dist[(short)type])
            return;

        auto ptype2 = ptype[atype2];
        sync_data sync_diff1;
        sync_diff1.back() += back1;
        sync_diff1.side_all() += side1;
        sync_diff1.side_polar() += side1 && (ptype2 == POLAR);
        sync_diff1.side_hydrophobic() += side1 && (ptype2 == HYDROPHOBIC);

        sync_data sync1_after_formation = sync[i1] - sync_diff1;
        if (!sync1_after_formation.is_valid())
            return;

        auto ptype1 = ptype[atype1];
        sync_data sync_diff2;
        sync_diff2.back() += back2;
        sync_diff2.side_all() += side2;
        sync_diff2.side_polar() += side2 && (ptype1 == POLAR);
        sync_diff2.side_hydrophobic() += side2 && (ptype1 == HYDROPHOBIC);

        sync_data sync2_after_formation = sync[i2] - sync_diff2;
        if (!sync2_after_formation.is_valid())
            return;

#pragma omp critical
        candidates->emplace(i1, i2, idx, type, sync_diff1, sync_diff2);
    }

    void sift_candidates::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < free_pairs->extent(); ++idx) {
            iter(idx);
        }
    }

    void sift_candidates::omp_prep() const {
        candidates->clear();
    }
}