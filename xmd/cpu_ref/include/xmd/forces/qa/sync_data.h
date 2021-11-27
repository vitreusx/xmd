#pragma once
#include <xmd/utils/meta.h>

namespace xmd {
    class sync_data_cref {
    public:
        sync_data_cref(int8_t const& back, int8_t const& side_all,
            int8_t const& side_polar, int8_t const& side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

    public:
        int8_t const &back, &side_all, &side_polar, &side_hydrophobic;
    };

    class sync_data_ref {
    public:
        sync_data_ref(int8_t const& back, int8_t const& side_all,
            int8_t const& side_polar, int8_t const& side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

        inline auto& operator=(sync_data_cref const& other) {
            back = other.back;
            side_all = other.side_all;
            side_polar = other.side_polar;
            side_hydrophobic = other.side_hydrophobic;
            return *this;
        }

        inline operator sync_data_cref() const {
            return { back, side_all, side_polar, side_hydrophobic };
        }

    public:
        int8_t &back, &side_all, &side_polar, &side_hydrophobic;
    };

    struct sync_data_array {
        int8_t *back, *side_all, *side_polar, *side_hydrophobic;
        int size;

        sync_data_ref operator[](int idx) {
            return { back[idx], side_all[idx], side_polar[idx],
                     side_hydrophobic[idx] };
        }

        sync_data_cref operator[](int idx) const {
            return { back[idx], side_all[idx], side_polar[idx],
                     side_hydrophobic[idx] };
        }
    };
}