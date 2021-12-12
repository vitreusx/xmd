#include "forces/pid/bundle.h"
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::pid {

    void update_pid_bundles::operator()() const {
        bundles->clear();

        auto min_norm_inv = (real)1.0 / (cutoff + nl->orig_pad);

        for (int pair_idx = 0; pair_idx < nl->particle_pairs.size; ++pair_idx) {
            auto idx1 = nl->particle_pairs.i1[pair_idx];
            auto idx2 = nl->particle_pairs.i2[pair_idx];
            auto r1 = r[idx1], r2 = r[idx2];
            if (norm_inv(box->ray(r1, r2)) > min_norm_inv) {
                auto prev1 = prev[idx1], next1 = next[idx1];
                auto prev2 = prev[idx2], next2 = next[idx2];
                auto atype1 = atype[idx1], atype2 = atype[idx2];

                int bundle_idx = bundles->push_back();
                bundles->i1p[bundle_idx] = prev1;
                bundles->i1[bundle_idx] = idx1;
                bundles->i1n[bundle_idx] = next1;
                bundles->i2p[bundle_idx] = prev2;
                bundles->i2[bundle_idx] = idx2;
                bundles->i2n[bundle_idx] = next2;
                int16_t type = (int16_t)atype1 *
                               (int16_t)amino_acid::NUM_AA + (int16_t)atype2;
                bundles->type[bundle_idx] = type;
            }
        }
    }

    void update_pid_bundles::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        prev = vm_inst.find<vector<int>>("prev").to_array();
        next = vm_inst.find<vector<int>>("next").to_array();
        atype = vm_inst.find<vector<amino_acid>>("atype").to_array();
        box = &vm_inst.find<xmd::box<vec3r>>("box");
        nl = &vm_inst.find<nl::nl_data>("nl_data");
        bundles = &vm_inst.find<pid_bundle_vector>("pid_bundles");

        cutoff = vm_inst.find_or<real>("pid_cutoff", [&]() -> auto& {
            auto& cutoff_ = vm_inst.emplace<real>("pid_cutoff", 0.0);

            auto& variants = vm_inst.find<lj_variants>("lj_variants");
            cutoff_ = max(cutoff_, variants.bb.cutoff());
            cutoff_ = max(cutoff_, variants.bs.cutoff());
            cutoff_ = max(cutoff_, variants.sb.cutoff());
            for (int ss_idx = 0; ss_idx < variants.ss.size; ++ss_idx) {
                auto ss = variants.ss[ss_idx];
                cutoff_ = max(cutoff_, ss.cutoff());
            }

            return cutoff_;
        });

        auto& max_cutoff = vm_inst.find<real>("max_cutoff");
        max_cutoff = max(max_cutoff, cutoff);
    }

    int pid_bundle_vector::push_back() {
        i1p.push_back();
        i1.push_back();
        i1n.push_back();
        i2p.push_back();
        i2.push_back();
        i2n.push_back();
        type.push_back();
        return size++;
    }

    void pid_bundle_vector::clear() {
        i1p.clear();
        i1.clear();
        i1n.clear();
        i2p.clear();
        i2.clear();
        i2n.clear();
        type.clear();
        size = 0;
    }

    pid_bundle_span pid_bundle_vector::to_span() {
        pid_bundle_span s;
        s.i1p = i1p.data();
        s.i1 = i1.data();
        s.i1n = i1n.data();
        s.i2p = i2p.data();
        s.i2 = i2.data();
        s.i2n = i2n.data();
        s.size = size;
        return s;
    }
}