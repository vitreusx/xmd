#pragma once
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/math.h>
#include <xmd/nl/nl_data.h>
#include <xmd/forces/primitives/lj.h>

namespace xmd {
    struct pauli_pair_span {
        array<int> i1, i2;
        int size;
    };

    struct pauli_pair_vector {
        vector<int> i1, i2;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            size = 0;
        }

        inline auto to_span() const {
            pauli_pair_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.size = size;
            return s;
        }
    };

    class update_pauli_pairs {
    public:
        float r_excl;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        pauli_pair_vector *pairs;

    public:
        inline void operator()() const {
            pairs->clear();

            float min_rnorm = 1.0f / (r_excl + nl->pad);
            for (int idx = 0; idx < nl->neighbor_cells->size(); ++idx) {
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
                        auto r12_rn = rnorm(r12);

                        if (r12_rn > min_rnorm) {
                            int pair_idx = pairs->push_back();
                            pairs->i1[pair_idx] = part_idx1;
                            pairs->i2[pair_idx] = part_idx2;
                        }
                    }
                }
            }
        }
    };

    class eval_pauli_exclusion_forces {
    public:
        float depth, r_excl;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        pauli_pair_span pairs;
        float *V;

    public:
        inline void operator()() const {
            for (int idx = 0; idx < pairs.size; ++idx) {
                auto i1 = pairs.i1[idx], i2 = pairs.i2[idx];
                auto r1 = r[i1], r2 = r[i2];
                auto r12 = box->ray(r1, r2);
                auto r12_rn = norm_inv(r12);

                auto within = (1.0f < r12_rn * r_excl);
                if (within) {
                    auto r12_u = r12 * r12_rn;
                    auto [V_, dV_dr] = lj(depth, r_excl)(r12_rn);

                    *V += V_;
                    F[i1] -= r12_u * dV_dr;
                    F[i2] += r12_u * dV_dr;
                }
            }
        }
    };
}
