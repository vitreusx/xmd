#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/types/scalar.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "neighbor_cell.h"
#include "pair.h"

namespace xmd::nl {
    class spatial_data {
    public:
        int num_cells, num_items;
        vector<int> item_cell_idx, item_index_buckets;
        vector<int> cell_bucket_begin, cell_bucket_size;

        explicit spatial_data(int num_items = 0);
        void reinit(int num_cells);
        void validate() const;

        inline std::pair<int, int> range(int cell_idx) const {
            auto beg = cell_bucket_begin[cell_idx];
            auto end = beg + cell_bucket_size[cell_idx];
            return std::make_pair(beg, end);
        }
    };

    class nl_data {
    public:
        vector<nl_pair> particle_pairs;
        real orig_pad, ref_t;
        box<vec3r> orig_box;
        vector<vec3r> orig_r;
    };
}