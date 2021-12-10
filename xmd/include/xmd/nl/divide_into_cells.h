#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "data.h"
#include <xmd/vm/vm.h>

namespace xmd::nl {
    class divide_into_cells: public vm_aware {
    public:
        vec3r_array r, orig_r;
        box<vec3r> *box, *orig_box;
        nl_data *data;
        int num_particles;
        real *cutoff, *pad;
        bool *invalid;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}