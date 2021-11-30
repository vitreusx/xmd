#pragma once
#include "lambda.h"
#include "bundle.h"
#include <xmd/forces/primitives/lj.h>
#include <xmd/forces/primitives/sink_lj.h>

namespace xmd::pid {
    class eval_pid_forces {
    public:
        lambda_func bb_plus_lam, bb_minus_lam;
        lj bb_plus_lj, bb_minus_lj;

        lambda_func_array ss_lams;
        sink_lj_array ss_sink_ljs;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        pid_bundle_span bundles;
        float *V;

    public:
        void operator()();
    };
}