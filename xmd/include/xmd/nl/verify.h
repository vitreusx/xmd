#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>
#include "data.h"

namespace xmd::nl {
    class verify: public vm_aware {
    public:
        vec3r_array r;
        box<vec3r> *box;
        bool *invalid, *first_time;
        int num_particles;
        nl_data *data;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}