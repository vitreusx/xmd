#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/types/scalar.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include "neighbor_cell.h"

namespace xmd::nl {
    class nl_data {
    public:
        vector<int> cell_idx_for_particle, particle_index_groups;
        vector<int> cell_begin, cell_num_part;
        neighbor_cell_list neighbor_cells;
        real pad;
        int num_cells;
    };

    template<typename Function>
    class iter_over_pairs {
    public:
        Function const& f;
        nl_data *nl;
        vec3r_array r;
        real cutoff;
        box<vec3r> *box;

        explicit iter_over_pairs(Function const& f):
            f {f} {};

    public:
        void operator()() const {
            real min_req_rnorm = (real)1.0 / (cutoff + nl->pad);
            for (int pair_idx = 0; pair_idx < nl->neighbor_cells.size(); ++pair_idx) {
                auto cell_idx1 = nl->neighbor_cells.cell_idx1[pair_idx];
                auto cell_beg1 = nl->cell_begin[cell_idx1];
                auto cell_end1 = cell_beg1 + nl->cell_num_part[cell_idx1];

                auto cell_idx2 = nl->neighbor_cells.cell_idx2[pair_idx];
                auto cell_beg2 = nl->cell_begin[cell_idx2];
                auto cell_end2 = cell_beg2 + nl->cell_num_part[cell_idx2];

                for (int group_idx1 = cell_beg1; group_idx1 < cell_end1; ++group_idx1) {
                    int part_idx1 = nl->particle_index_groups[group_idx1];
                    auto r1 = r[part_idx1];

                    for (int group_idx2 = cell_beg2; group_idx2 < cell_end2; ++group_idx2) {
                        int part_idx2 = nl->particle_index_groups[group_idx2];
                        auto r2 = r[part_idx2];

                        auto r12 = box->ray(r1, r2);
                        auto r12_rn = norm_inv(r12);
                        if (r12_rn > min_req_rnorm)
                            f(part_idx1, part_idx2);
                    }
                }
            }
        }
    };
}