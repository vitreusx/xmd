#pragma once
#include "lj.h"
#include "sink_lj.h"
#include <xmd/vm/vm.h>

namespace xmd {
    class lj_variants: public vm_aware {
    public:
        lj bb, bs, sb;
        sink_lj_array ss;

        sink_lj operator[](int idx) const;
        void init_from_vm(vm& vm_inst) override;
    };
}