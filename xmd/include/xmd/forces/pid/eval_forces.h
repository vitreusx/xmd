#pragma once
#include "lambda.h"
#include "bundle.h"
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>
#include <xmd/vm/vm.h>
#include <xmd/forces/primitives/lj_variants.h>

namespace xmd::pid {
    class eval_pid_forces: public vm_aware {
    public:
        struct conf_t {
            lambda_func bb_plus_lam, bb_minus_lam, ss_lam;
            lj bb_plus_lj, bb_minus_lj;
            sink_lj_array ss_ljs;
        } conf;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        pid_bundle_vector *bundles;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()();
        void omp_async() const;
    };
}