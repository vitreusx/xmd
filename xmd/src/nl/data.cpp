#include "nl/data.h"

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
}