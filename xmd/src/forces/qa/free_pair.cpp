#include "forces/qa/free_pair.h"
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::qa {

    void update_free_pairs::operator()() const {
        pairs->clear();

        auto cutoff = max_formation_min_dist;
        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size; ++pair_idx) {
            auto i1 = nl->particle_pairs.i1[pair_idx];
            auto i2 = nl->particle_pairs.i2[pair_idx];
            auto r1 = r[i1], r2 = r[i2];
            if (norm_inv(box->ray(r1, r2)) > min_norm_inv) {
                int free_pair_idx = pairs->add();
                pairs->i1[free_pair_idx] = i1;
                pairs->i2[free_pair_idx] = i2;
            }
        }
    }

    void update_free_pairs::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl_data");
        pairs = &vm_inst.find<free_pair_set>("qa_free_pairs");

        max_formation_min_dist = vm_inst.find_or<real>("max_formation_min_dist",
            [&]() -> auto& {
                auto& dist_ = vm_inst.emplace<real>("max_formation_min_dist", 0.0);

                auto& variants = vm_inst.find<lj_variants>("lj_variants");
                dist_ = max(dist_, variants.bb.r_min);
                dist_ = max(dist_, variants.bs.r_min);
                dist_ = max(dist_, variants.sb.r_min);
                for (int ss_idx = 0; ss_idx < variants.ss.size; ++ss_idx) {
                    auto ss = variants.ss[ss_idx];
                    dist_ = max(dist_, ss.r_max);
                }

                return dist_;
            });

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        max_cutoff = max(max_cutoff, max_formation_min_dist);
    }
}