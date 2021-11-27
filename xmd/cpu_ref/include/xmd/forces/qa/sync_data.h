#pragma once

namespace xmd {
    template<typename T>
    struct gen_sync_data {
    public:
        gen_sync_data() = default;
        gen_sync_data(T back, T side_all, T side_polar, T side_hydrophobic):
            back{back}, side_all{side_all}, side_polar{side_polar},
            side_hydrophobic{side_hydrophobic} {};

        template<typename U>
        inline auto& operator+=(gen_sync_data<U> const& diff) {
            back += diff.back;
            side_all += diff.side_all;
            side_polar += diff.side_polar;
            side_hydrophobic += diff.side_hydrophobic;
            return *this;
        }

        template<typename U>
        inline auto& operator-=(gen_sync_data<U> const& diff) {
            back -= diff.back;
            side_all -= diff.side_all;
            side_polar -= diff.side_polar;
            side_hydrophobic -= diff.side_hydrophobic;
            return *this;
        }

    public:
        T back, side_all, side_polar, side_hydrophobic;
    };

    template<typename T, typename U>

}