#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>
#include "data.h"

namespace xmd::nl {
    class verify: public vm_aware {
    public:
        const_array<vec3r> r;
        box const *box;
        bool *invalid, *first_time;
        int num_particles;
        nl_data const *data;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}