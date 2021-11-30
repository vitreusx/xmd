#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd::qa {
    struct nh_bundle_span {
        array<int> iprev, icur, inext;
        int size;
    };

    class precompute_nh {
    public:
        vec3f_array r, n, h;
        box<vec3f> *box;
        nh_bundle_span bundles;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < bundles.size; ++idx) {
                auto iprev = bundles.iprev[idx], icur = bundles.icur[idx],
                    inext = bundles.inext[idx];

                auto rprev = r[iprev], rcur = r[icur], rnext = r[inext];
                auto v1 = rcur - rprev, v2 = rnext - rcur;
                n[icur] = unit(v2 - v1);
                h[icur] = unit(cross(v2, v1));
            }
        }
    };
}