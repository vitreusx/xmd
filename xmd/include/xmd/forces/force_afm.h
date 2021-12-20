#pragma once
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct force_afm_bundle_span {
        array<int> pulled_idx;
        vec3r_array pull_force;
        int size;
    };

    class force_afm_bundle_vector {
    public:
        vector<int> pulled_idx;
        vec3r_vector pull_force;
        int size;

        explicit force_afm_bundle_vector(int n = 0);
        int push_back();
        force_afm_bundle_span to_span();
    };

    class eval_force_afm_forces: public vm_aware {
    public:
        vec3r_array F;
        force_afm_bundle_span bundles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}