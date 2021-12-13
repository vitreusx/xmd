#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/vm/vm.h>
#include <taskflow/taskflow.hpp>

namespace xmd {
    class heurestic_dihedral_type {
    public:
        heurestic_dihedral_type() = default;
        heurestic_dihedral_type(amino_acid const& a2, amino_acid const& a3);

        explicit constexpr operator int8_t();

    private:
        explicit constexpr heurestic_dihedral_type(int8_t val);

        int8_t val = 0;
    };

    struct heurestic_dihedral_span {
        array<int> i1, i2, i3, i4;
        array<heurestic_dihedral_type> type;
        int size;
    };

    class heurestic_dihedral_vector {
    public:
        explicit heurestic_dihedral_vector(int n = 0);

        heurestic_dihedral_span to_span();

        vector<int> i1, i2, i3, i4;
        vector<heurestic_dihedral_type> type;
        int size;
    };

    class eval_heurestic_dihedral_forces: public vm_aware {
    public:
        static constexpr int NUM_TERMS = 6, NUM_TYPES = 9;
        real coeffs[NUM_TERMS][NUM_TYPES];

        vec3r_array r, F;
        heurestic_dihedral_span dihedrals;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void loop_iter(int idx) const;
        void operator()() const;
        tf::Task tf_impl(tf::Taskflow& taskflow) const;
    };
}
