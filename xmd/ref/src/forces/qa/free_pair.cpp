#include "forces/qa/free_pair.h"

namespace xmd::qa {

    void update_free_pairs::operator()() const {
        pairs->clear();

        real min_rnorm = 1.0f / (max_formation_min_dist + nl->pad);
        for (int idx = 0; idx < nl->neighbor_cells.size(); ++idx) {
            auto cell_idx1 = nl->neighbor_cells.cell_idx1[idx];
            auto cell_beg1 = nl->cell_begin[cell_idx1];
            auto cell_end1 = cell_beg1 + nl->cell_num_part[cell_idx1];

            auto cell_idx2 = nl->neighbor_cells.cell_idx2[idx];
            auto cell_beg2 = nl->cell_begin[cell_idx2];
            auto cell_end2 = cell_beg2 + nl->cell_num_part[cell_idx2];

            for (int part_idx1 = cell_beg1; part_idx1 < cell_end1; ++part_idx1) {
                auto r1 = r[part_idx1];
                for (int part_idx2 = cell_beg2; part_idx2 < cell_end2; ++part_idx2) {
                    auto r2 = r[part_idx2];
                    auto r12 = box->ray(r1, r2);
                    auto r12_rn = norm_inv(r12);

                    if (r12_rn > min_rnorm) {
                        int pair_idx = pairs->add();
                        pairs->i1[pair_idx] = part_idx1;
                        pairs->i2[pair_idx] = part_idx2;
                    }
                }
            }
        }
    }
}