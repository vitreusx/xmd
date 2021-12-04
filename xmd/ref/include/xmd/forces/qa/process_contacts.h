#pragma once
#include <xmd/forces/primitives/lj.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact.h"
#include "sync_data.h"

namespace xmd::qa {
    class process_contacts {
    public:
        lj_array ljs;
        real cycle_time, cycle_time_inv, breaking_factor;

    public:
        vec3r_array r, F;
        box<vec3r> *box;
        contact_set *contacts;
        real *V, *t;
        sync_data_array sync;

    public:
        void operator()() const;
    };
}