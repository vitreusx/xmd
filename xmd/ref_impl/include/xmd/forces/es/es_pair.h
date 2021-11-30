#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>

namespace xmd {
    struct es_pair_span {
        array<int> i1, i2;
        array<float> q1_q2;
        int size;
    };

    struct es_pair_vector {
        vector<int> i1, i2;
        vector<float> q1_q2;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            q1_q2.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            q1_q2.clear();
            size = 0;
        }

        inline auto to_span() const {
            es_pair_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.q1_q2 = q1_q2.data();
            s.size = size;
            return s;
        }
    };

    class update_es_pairs {
    public:
        float cutoff;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        es_pair_vector *pairs;

    public:
        inline void operator()() const {
            pairs->clear();

            float min_rnorm = 1.0f / (cutoff + nl->pad);
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
}