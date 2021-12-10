#pragma once
#include <xmd/types/vec3.h>
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct velocity_afm_bundle_span {
        array<int> pulled_idx;
        vec3r_array afm_orig, afm_vel;
        int size;
    };

    class velocity_afm_bundle_vector {
    public:
        vector<int> pulled_idx;
        vec3r_vector afm_orig, afm_vel;
        int size;

        explicit velocity_afm_bundle_vector(int n);
        velocity_afm_bundle_span to_span();
    };

    class eval_velocity_afm_forces: public vm_aware {
    public:
        harmonic afm_force;

    public:
        vec3r_array r, F;
        real *t;
        velocity_afm_bundle_span bundles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}