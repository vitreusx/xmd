#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"
#include <xmd/vm/vm.h>
#include <taskflow/taskflow.hpp>

namespace xmd {
    class eval_const_es_forces: public vm_aware {
    public:
        real permittivity, screen_dist_inv;
        real V_factor;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        es_pair_span es_pairs;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void loop_iter(int idx) const;
        void operator()() const;
        tf::Task tf_impl(tf::Taskflow& taskflow) const;
    };
}
