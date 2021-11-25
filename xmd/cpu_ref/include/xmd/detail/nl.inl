#pragma once

namespace xmd {
    void verify_integrity::operator()() const {
        auto max_part_move = 1.0f;
        for (size_t idx = 0; idx < r.size(); ++idx) {
            auto move = norm(r[idx] - orig_r[idx]);
            max_part_move = fmaxf(max_part_move, move);
        }

        auto box_move = l1_norm(*box - *orig_box);

        return 2.0f * max_part_move + box_move < *pad;
    }
}