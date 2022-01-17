#pragma once
#include "lj.h"
#include "sink_lj.h"
#include <xmd/ctx/context.h>
#include <xmd/params/yaml_fs_node.h>

namespace xmd {
    class lj_variants {
    public:
        lj_variants() = default;
        explicit lj_variants(yaml_fs_node const& p);

        lj bb, bs, sb;
        const_span<sink_lj> ss;

        inline sink_lj operator[](int idx) const {
            switch (idx) {
            case 0: return sink_lj(bb);
            case 1: return sink_lj(bs);
            case 2: return sink_lj(sb);
            default: return ss[idx-3];
            };
        }
    };
}