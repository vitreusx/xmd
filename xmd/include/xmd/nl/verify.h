#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>

namespace xmd::nl {
    class verify: public vm_aware {
    public:
        vec3r_array r, orig_r;
        box<vec3r> *box, *orig_box;
        bool *invalid, *first_time;
        int num_particles;
        real *pad;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}