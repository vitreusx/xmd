#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/types/amino_acid.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct native_angle_span {
        array<int> i1, i2, i3;
        array<real> nat_theta;
        int size;
    };

    class native_angle_vector {
    public:
        explicit native_angle_vector(int n = 0);
        native_angle_span to_span();

        vector<int> i1, i2, i3;
        vector<real> nat_theta;
        int size;
    };

    class eval_native_angle_forces: public vm_aware {
    public:
        real k;

    public:
        real *V;
        vec3r_array r, F;
        native_angle_span angles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
