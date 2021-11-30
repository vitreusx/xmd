#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/types/amino_acid.h>
#include <xmd/model/box.h>

namespace xmd::pid {
    struct pid_bundle_span {
        array<int> i1p, i1, i1n;
        array<int> i2p, i2, i2n;
        array<int16_t> type;
        int size;
    };

    class pid_bundle_vector {
    public:
        vector<int> i1p, i1, i1n;
        vector<int> i2p, i2, i2n;
        vector<int16_t> type;
        int size;

        inline int push_back() {
            i1p.push_back();
            i1.push_back();
            i1n.push_back();
            i2p.push_back();
            i2.push_back();
            i2n.push_back();
            type.push_back();
            return size++;
        }

        inline void clear() {
            i1p.clear();
            i1.clear();
            i1n.clear();
            i2p.clear();
            i2.clear();
            i2n.clear();
            type.clear();
            size = 0;
        }

        inline auto to_span() const {
            pid_bundle_span s;
            s.i1p = i1p.data();
            s.i1 = i1.data();
            s.i1n = i1n.data();
            s.i2p = i2p.data();
            s.i2 = i2.data();
            s.i2n = i2n.data();
            s.size = size;
            return s;
        }
    };

    class update_pid_bundles {
    public:
        float cutoff;

    public:
        vec3f_array r;
        array<int> prev, next;
        array<amino_acid> atype;
        box<vec3f> *box;
        nl::nl_data *nl;
        pid_bundle_vector *bundles;

    public:
        inline void operator()() const {
            bundles->clear();

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
                    auto prev1 = prev[part_idx1], next1 = next[part_idx1];
                    auto atype1 = atype[part_idx1];

                    for (int part_idx2 = cell_beg2; part_idx2 < cell_end2; ++part_idx2) {
                        auto r2 = r[part_idx2];
                        auto r12 = box->ray(r1, r2);
                        auto r12_rn = rnorm(r12);

                        if (r12_rn > min_rnorm) {
                            auto prev2 = prev[part_idx1], next2 = next[part_idx2];
                            auto atype2 = atype[part_idx2];

                            int bundle_idx = pairs->push_back();
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
    };
}