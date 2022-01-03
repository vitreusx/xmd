#pragma once
#include "lj.h"
#include "sink_lj.h"
#include <xmd/vm/vm.h>

namespace xmd {
    class lj_variants: public vm_aware {
    public:
        lj bb, bs, sb;
        array<sink_lj> ss;

        inline sink_lj operator[](int idx) const {
            switch (idx) {
            case 0: return sink_lj(bb);
            case 1: return sink_lj(bs);
            case 2: return sink_lj(sb);
            default: return ss[idx-3];
            };
        }

        void init_from_vm(vm& vm_inst) override;
    };
}