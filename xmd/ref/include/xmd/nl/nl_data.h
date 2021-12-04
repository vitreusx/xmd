#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/types/scalar.h>
#include "neighbor_cell.h"

namespace xmd::nl {
    struct nl_data {
        array<int> part_cell_idx, cell_sorted_indices;
        vector<int> cell_begin, cell_num_part;
        neighbor_cell_list neighbor_cells;
        real pad;
        int num_cells;
    };
}