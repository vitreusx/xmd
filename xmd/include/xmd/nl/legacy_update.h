#pragma once
#include "data.h"

#include <xmd/types/vec3.h>
#include <xmd/model/box.h>


namespace xmd::nl {
    class legacy_update {
    public:
        real pad_factor;

    public:
        const_array<vec3r> r;
        box const *box;
        nl_data *data;
        int num_particles;
        real const *max_cutoff, *t;
        bool *invalid;
        const_array<int> chain_idx, seq_idx;
        const_span<nl_pair> all_nat_cont, all_nat_ss;



    public:
        void operator()() const;
    };
}