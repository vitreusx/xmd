#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vec3.h>
#include <xmd/types/amino_acid.h>
#include <xmd/vm/vm.h>

namespace xmd {
    class heurestic_angle_type {
    public:
        heurestic_angle_type() = default;
        heurestic_angle_type(amino_acid const& a1, amino_acid const& a2) ;

        explicit constexpr operator int8_t();

    private:
        explicit constexpr heurestic_angle_type(int8_t val);

        int8_t val = 0;
    };

    struct heurestic_angle_span {
        array<int> i1, i2, i3;
        array<heurestic_angle_type> type;
        int size;
    };

    class heurestic_angle_vector {
    public:
        explicit heurestic_angle_vector(int n = 0);

        vector<int> i1, i2, i3;
        vector<heurestic_angle_type> type;
        int size;

        heurestic_angle_span to_span();
    };

    class eval_heurestic_angle_forces: public vm_aware {
    public:
        static constexpr int POLY_DEG = 6, NUM_TYPES = 9;
        real poly_coeffs[POLY_DEG+1][NUM_TYPES];

    public:
        vec3r_array r, F;
        heurestic_angle_span angles;
        real *V;

        void bind_to_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}
