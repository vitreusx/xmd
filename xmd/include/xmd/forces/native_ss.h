#pragma once
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/model/box.h>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/nl/data.h>
#include <xmd/types/vec3.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct nat_ssbond_span {
        array<int> i1, i2;
        int size;
    };

    struct nat_ssbond_vector {
        vector<int> i1, i2;
        int size;

        explicit nat_ssbond_vector(int n = 0);

        int push_back();
        void clear();
        nat_ssbond_span to_span();
    };

    class eval_nat_ssbond_forces: public vm_aware {
    public:
        real H1, nat_r;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        nat_ssbond_span ssbonds;
        real *V;

        void init_from_vm(vm& vm_inst) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_nat_ssbonds: public vm_aware {
    public:
        real cutoff;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl::nl_data *nl;
        nat_ssbond_vector *all_ssobnds, *ssbonds;
        eval_nat_ssbond_forces *eval;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}