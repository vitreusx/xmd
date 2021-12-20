#include "nl/data.h"
#include <stdexcept>

namespace xmd::nl {
    spatial_data::spatial_data(int num_items) {
        this->num_items = num_items;
        item_cell_idx = item_index_buckets = vector<int>(num_items);
    }

    void spatial_data::reinit(int num_cells) {
        this->num_cells = num_cells;
        cell_bucket_begin.resize(num_cells);
        cell_bucket_size.resize(num_cells);
    }

    void spatial_data::validate() const {
        for (int part_idx = 0; part_idx < num_items; ++part_idx) {
            auto cell_idx = item_cell_idx[part_idx];
            if (!(0 <= cell_idx && cell_idx < num_cells))
                throw;

            auto kth = item_index_buckets[part_idx];
            if (!(0 <= kth && kth < num_items))
                throw;
        }

        int total_size = 0;

        for (int cell_idx = 0; cell_idx < num_cells; ++cell_idx) {
            auto beg = cell_bucket_begin[cell_idx];
            if (!(0 <= beg && beg <= num_items))
                throw;

            auto size = cell_bucket_size[cell_idx];
            if (!(0 <= size && size <= num_items && beg + size <= num_items))
                throw;

            if (beg == num_items && size != 0)
                throw;

            total_size += size;
        }

        if (total_size != num_items)
            throw;
    }
}