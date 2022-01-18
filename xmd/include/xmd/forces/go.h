#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/native_contact.h>
#include <xmd/model/box.h>
#include <xmd/utils/math.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/nl/data.h>


namespace xmd {
    class eval_go_forces {
    public:
        real depth;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        vector<nat_cont> const *contacts;
        real *V;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };

    class update_go_contacts {
    public:
        const_array<vec3r> r;
        box const *box;
        nl::nl_data const *nl;
        vector<nat_cont> const *all_contacts;
        vector<nat_cont> *contacts;



    public:
        void operator()() const;
    };
}
