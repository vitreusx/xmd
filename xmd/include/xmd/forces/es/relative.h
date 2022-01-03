#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "es_pair.h"
#include <xmd/vm/vm.h>

namespace xmd {
    class eval_relative_es_forces: public vm_aware  {
    public:
        real factor, screen_dist_inv;
        real V_factor;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box<vec3r> const *box;
        vector<es_pair> const* es_pairs;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_relative_es: public update_es_base {
    public:
        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
