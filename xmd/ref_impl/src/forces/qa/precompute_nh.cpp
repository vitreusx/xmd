#include "forces/qa/precompute_nh.h"

namespace xmd::qa {
    void precompute_nh::operator()() const {
        for (int idx = 0; idx < bundles.size; ++idx) {
            auto iprev = bundles.iprev[idx], icur = bundles.icur[idx],
                inext = bundles.inext[idx];

            auto rprev = r[iprev], rcur = r[icur], rnext = r[inext];
            auto v1 = rcur - rprev, v2 = rnext - rcur;
            n[icur] = unit(v2 - v1);
            h[icur] = unit(cross(v2, v1));
        }
    }
}