#include "forces/native_ss.h"

namespace xmd {

    void update_nat_ssbonds::operator()() const {
        pairs->clear();

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
                for (int part_idx2 = cell_beg2; part_idx2 < cell_end2; ++part_idx2) {
                    auto r2 = r[part_idx2];
                    auto r12 = box->ray(r1, r2);
                    auto r12_rn = norm_inv(r12);

                    if (r12_rn > min_rnorm) {
                        int pair_idx = pairs->push_back();
                        pairs->i1[pair_idx] = part_idx1;
                        pairs->i2[pair_idx] = part_idx2;
                    }
                }
            }
        }
    }

    void eval_nat_ssbond_forces::operator()() const {
        for (int idx = 0; idx < bonds.size; ++idx) {
            auto cys_i1 = bonds.i1[idx], cys_i2 = bonds.i2[idx];
            auto r1 = r[cys_i1], r2 = r[cys_i2];
            auto r12 = box->ray(r1, r2);

            auto r12_n = norm(r12);
            auto [V_, dV_dr] = harmonic(H1, 0.0f, nat_r)(r12_n);

            auto r12_u = r12 / r12_n;
            *V += V_;
            F[cys_i1] += dV_dr * r12_u;
            F[cys_i2] -= dV_dr * r12_u;
        }
    }
}