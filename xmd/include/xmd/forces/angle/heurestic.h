#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class heur_ang_type {
    public:
        heur_ang_type() = default;
        heur_ang_type(amino_acid const& a1, amino_acid const& a2) ;

        explicit constexpr operator int8_t();

    private:
        explicit constexpr heur_ang_type(int8_t val);

        int8_t val = 0;
    };
}

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() heur_angle
#define FIELDS() int,i1,int,i2,int,i3,heur_ang_type,type

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd {
    class eval_heurestic_angle_forces: public vm_aware {
    public:
        static constexpr int POLY_DEG = 6, NUM_TYPES = 9;
        real poly_coeffs[POLY_DEG+1][NUM_TYPES];

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        const_span<heur_angle> angles;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
