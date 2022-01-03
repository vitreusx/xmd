#pragma once
#include <xmd/types/amino_acid.h>
#include <xmd/types/vec3.h>
#include <xmd/types/array.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class heur_dih_type {
    public:
        heur_dih_type() = default;

        heur_dih_type(amino_acid const &a2, amino_acid const &a3);

        explicit constexpr operator int8_t();

    private:
        explicit constexpr heur_dih_type(int8_t val);

        int8_t val = 0;
    };
}

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() heur_dih
#define FIELDS() int,i1,int,i2,int,i3,int,i4,heur_dih_type,type

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_heurestic_dihedral_forces: public vm_aware {
    public:
        static constexpr int NUM_TERMS = 6, NUM_TYPES = 9;
        real coeffs[NUM_TERMS][NUM_TYPES];

        const_array<vec3r> r;
        array<vec3r> F;
        const_span<heur_dih> dihedrals;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
