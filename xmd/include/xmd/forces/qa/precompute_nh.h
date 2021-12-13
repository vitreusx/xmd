#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>
#include <taskflow/taskflow.hpp>

namespace xmd::qa {
    struct nh_bundle_span {
        array<int> iprev, icur, inext;
        int size;
    };

    struct nh_bundle_vector {
        vector<int> iprev, icur, inext;
        int size;

        explicit nh_bundle_vector(int n = 0);

        nh_bundle_span to_span();
    };

    class precompute_nh: public vm_aware {
    public:
        vec3r_array r, n, h;
        box<vec3r> *box;
        nh_bundle_span bundles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void loop_iter(int idx) const;
        void operator()() const;
        tf::Task tf_impl(tf::Taskflow& taskflow) const;
    };
}