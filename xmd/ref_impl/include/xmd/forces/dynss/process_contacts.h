#pragma once
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "contact.h"
#include <xmd/forces/qa/sync_data.h>

namespace xmd::dynss {
    class process_contacts {
    public:
        harmonic disulfide_force;
        float cycle_time, cycle_time_inv;
        float optimal_dist, max_dist_deviation;
        int min_req_num_of_neighbours;
        qa::sync_data ssbond_sync_diff;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        contact_set *contacts;
        float *V, *t;
        qa::sync_data_array sync;
        array<int> num_neighbours;

    public:
        inline void operator()() const;
    };
}