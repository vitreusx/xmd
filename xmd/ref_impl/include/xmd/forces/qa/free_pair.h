#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>
#include <xmd/nl/nl_data.h>

namespace xmd::qa {
    class free_pair_set {
    public:
        inline bool has_item(int idx) const {
            return has_item_[idx];
        }

        int extent() const {
            return extent_;
        }

        int add() {
            if (!vacant_slots.empty()) {
                int slot_idx = vacant_slots.pop();
                has_item_[slot_idx] = true;
                return slot_idx;
            }
            else {
                i1.emplace_back();
                i2.emplace_back();

                has_item_.push_back(true);
                return extent_++;
            }
        }

        void remove(int slot_idx) {
            has_item_[slot_idx] = false;
            vacant_slots.push(slot_idx);
        }

        void clear() {
            i1.clear();
            i2.clear();
        }

        vector<int> i1, i2;

    private:
        int extent_;
        vector<bool> has_item_;
        cyclic_buffer<int> vacant_slots;
    };

    class update_free_pairs {
    public:
        float max_formation_min_dist;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        update_free_pairs *pairs;

    public:
        inline void operator()() const {
            pairs->clear();

            float min_rnorm = 1.0f / (max_formation_min_dist + nl->pad);
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
                            int pair_idx = pairs->add();
                            pairs->i1[pair_idx] = part_idx1;
                            pairs->i2[pair_idx] = part_idx2;
                        }
                    }
                }
            }
        }
    };
}