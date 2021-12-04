#include "forces/pid/bundle.h"

namespace xmd::pid {

    void update_pid_bundles::operator()() const {
        bundles->clear();

        real min_rnorm = 1.0f / (cutoff + nl->pad);
        for (int idx = 0; idx < nl->neighbor_cells.size(); ++idx) {
            auto cell_idx1 = nl->neighbor_cells.cell_idx1[idx];
            auto cell_beg1 = nl->cell_begin[cell_idx1];
            auto cell_end1 = cell_beg1 + nl->cell_num_part[cell_idx1];

            auto cell_idx2 = nl->neighbor_cells.cell_idx2[idx];
            auto cell_beg2 = nl->cell_begin[cell_idx2];
            auto cell_end2 = cell_beg2 + nl->cell_num_part[cell_idx2];

            for (int part_idx1 = cell_beg1; part_idx1 < cell_end1; ++part_idx1) {
                auto r1 = r[part_idx1];
                auto prev1 = prev[part_idx1], next1 = next[part_idx1];
                auto atype1 = atype[part_idx1];

                for (int part_idx2 = cell_beg2; part_idx2 < cell_end2; ++part_idx2) {
                    auto r2 = r[part_idx2];
                    auto r12 = box->ray(r1, r2);
                    auto r12_rn = norm_inv(r12);

                    if (r12_rn > min_rnorm) {
                        auto prev2 = prev[part_idx1], next2 = next[part_idx2];
                        auto atype2 = atype[part_idx2];

                        int bundle_idx = bundles->push_back();
                        bundles->i1p[bundle_idx] = prev1;
                        bundles->i1[bundle_idx] = part_idx1;
                        bundles->i1n[bundle_idx] = next1;
                        bundles->i2p[bundle_idx] = prev2;
                        bundles->i2[bundle_idx] = part_idx2;
                        bundles->i2n[bundle_idx] = next2;
                        int16_t type = (int16_t)atype1 +
                                       (int16_t)amino_acid::NUM_AA * (int16_t)atype2;
                        bundles->type[bundle_idx] = type;
                    }
                }
            }
        }
    }
}