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
        float cycle_time, cycle_time_inv, breaking_factor;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        contact_set *contacts;
        float *V, *t;
        sync_data_array sync;

    public:
        void operator()() const;
    };
}