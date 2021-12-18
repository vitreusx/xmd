#include "forces/qa/precompute_nh.h"
#include <xmd/model/model.h>
#include <unordered_map>

namespace xmd::qa {
    void precompute_nh::operator()() const {
//#pragma omp taskloop default(none) nogroup
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto iprev = bundles.iprev[idx], icur = bundles.icur[idx],
                inext = bundles.inext[idx];

            auto rprev = r[iprev], rcur = r[icur], rnext = r[inext];
            auto v1 = rcur - rprev, v2 = rnext - rcur;
            n[icur] = unit(v2 - v1);
            h[icur] = unit(cross(v2, v1));
        }
    }

    void precompute_nh::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vec3r_vector>("r").to_array();
        auto num_particles = vm_inst.find<int>("num_particles");

        n = vm_inst.find_or_emplace<vec3r_vector>("qa_n",
            num_particles).to_array();

        h = vm_inst.find_or_emplace<vec3r_vector>("qa_h",
            num_particles).to_array();

        bundles = vm_inst.find_or<nh_bundle_vector>("nh_bundles", [&]() -> auto& {
            auto& xmd_model = vm_inst.find<xmd::model>("model");
            using res_map_t = std::unordered_map<xmd::model::residue*, int>;
            auto& res_map = vm_inst.find<res_map_t>("res_map");

            auto& bundles_ = vm_inst.emplace<nh_bundle_vector>("nh_bundles",
                xmd_model.angles.size());
            int bundle_idx = 0;
            for (auto const& angle: xmd_model.angles) {
                bundles_.iprev[bundle_idx] = res_map[angle.res1];
                bundles_.icur[bundle_idx] = res_map[angle.res2];
                bundles_.inext[bundle_idx] = res_map[angle.res3];
                ++bundle_idx;
            }

            return bundles_;
        }).to_span();
    }

    nh_bundle_span nh_bundle_vector::to_span() {
        nh_bundle_span span;
        span.iprev = iprev.to_array();
        span.icur = icur.to_array();
        span.inext = inext.to_array();
        span.size = size;
        return span;
    }

    nh_bundle_vector::nh_bundle_vector(int n):
        iprev(n), icur(n), inext(n), size(n) {};
}