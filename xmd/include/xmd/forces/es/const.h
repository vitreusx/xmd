#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"
#include <xmd/vm/vm.h>

namespace xmd {
    class eval_const_es_forces: public vm_aware {
    public:
        real permittivity, screen_dist_inv;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        es_pair_span es_pairs;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
