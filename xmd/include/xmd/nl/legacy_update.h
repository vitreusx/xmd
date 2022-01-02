#pragma once
#include "data.h"
#include <xmd/types/vector.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>

namespace xmd::nl {
    class legacy_update: public vm_aware {
    public:
        real pad_factor;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl_data *data;
        int num_particles;
        real *max_cutoff, *t;
        bool *invalid;
        array<int> chain_idx, seq_idx;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}