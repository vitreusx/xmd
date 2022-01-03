#include "forces/qa/precompute_nh.h"
#include <xmd/model/model.h>
#include <unordered_map>

namespace xmd::qa {
    void precompute_nh::operator()() const {
        for (int idx = 0; idx < num_particles; ++idx) {
            iter(idx);
        }
    }

    void precompute_nh::init_from_vm(vm &vm_inst) {
        r = vm_inst.find<vector<vec3r>>("r").data();
        num_particles = vm_inst.find<int>("num_particles");

        n = vm_inst.find_or_emplace<vector<vec3r>>("qa_n",
            num_particles).data();
        h = vm_inst.find_or_emplace<vector<vec3r>>("qa_h",
            num_particles).data();

        prev = vm_inst.find<vector<int>>("prev").data();
        next = vm_inst.find<vector<int>>("next").data();
    }

    void precompute_nh::iter(int idx) const {
        auto iprev = prev[idx], icur = idx, inext = next[idx];
        if (iprev < 0 || inext < 0) return;

        auto rprev = r[iprev], rcur = r[icur], rnext = r[inext];
        auto v1 = rcur - rprev, v2 = rnext - rcur;
        n[icur] = unit(v2 - v1);
        h[icur] = unit(cross(v2, v1));
    }

    void precompute_nh::omp_async() const {
#pragma omp for nowait schedule(dynamic, 512)
        for (int idx = 0; idx < num_particles; ++idx) {
            iter(idx);
        }
    }
}