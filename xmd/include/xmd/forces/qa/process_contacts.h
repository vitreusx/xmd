#pragma once
#include <xmd/forces/primitives/lj.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact.h"
#include "sync_data.h"

#include <xmd/forces/primitives/lj_variants.h>
#include "free_pair.h"

namespace xmd::qa {
    class process_contacts {
    public:
        lj_variants ljs;
        real cycle_time, cycle_time_inv, breaking_factor;
        real factor;

    public:
        const_array<vec3r> r;
        array<vec3r> F;
        box const *box;
        set<contact> *contacts;
        real *V, *t;
        array<sync_data> sync;
        set<free_pair> *free_pairs;



    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}