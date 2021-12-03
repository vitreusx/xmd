#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>

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

    class eval_heurestic_dihedral_forces {
    public:
        static constexpr int NUM_TERMS = 6, NUM_TYPES = 9;
        float coeffs[NUM_TERMS][NUM_TYPES];

        vec3f_span r, F;
        heurestic_dihedral_span dihedrals;
        float *V;

    public:
        void operator()() const;
    };
}
