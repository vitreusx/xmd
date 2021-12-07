#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>
#include <xmd/model/model.h>
#include <unordered_map>

namespace xmd {
    struct native_dihedral_span {
        array<int> i1, i2, i3, i4;
        array<real> nat_phi;
        int size;
    };

    class native_dihedral_vector {
    public:
        explicit native_dihedral_vector(int n = 0);

        native_dihedral_span to_span();

        vector<int> i1, i2, i3, i4;
        vector<real> nat_phi;
        int size;
    };

    class eval_native_dihedral_forces_base: public vm_aware {
    public:
        vec3r_array r, F;
        native_dihedral_span dihedrals;
        real *V;

        void bind_to_vm(vm& vm_inst) override;
    };
}