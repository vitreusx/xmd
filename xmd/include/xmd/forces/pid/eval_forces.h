#pragma once
#include "lambda.h"
#include "bundle.h"
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>
#include <xmd/vm/vm.h>

namespace xmd::pid {
    class eval_pid_forces: public vm_aware {
    public:
        lambda_func bb_plus_lam, bb_minus_lam;
        lj bb_plus_lj, bb_minus_lj;

        lambda_func_array ss_lams;
        sink_lj_array ss_sink_ljs;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        pid_bundle_span bundles;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()();
    };
}