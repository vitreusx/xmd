#pragma once
#include <xmd/types/amino_acid.h>

namespace xmd::qa {
    class contact_type {
    public:
        inline constexpr contact_type():
            val{decltype(val)(NONE())} {};

        static inline constexpr contact_type NONE() {
            return contact_type(0);
        }

        static inline constexpr contact_type BACK_BACK() {
            return contact_type(1);
        }

        static inline constexpr contact_type BACK_SIDE() {
            return contact_type(2);
        }

        static inline constexpr contact_type SIDE_BACK() {
            return contact_type(3);
        }

        static inline constexpr contact_type SIDE_SIDE(
            amino_acid const& a1, amino_acid const& a2) {

            int16_t val = (int16_t)4 + (int16_t)a1 + (int16_t)a2 * (int16_t)amino_acid::NUM_AA;
            return contact_type(val);
        }

        explicit constexpr inline operator int16_t() const {
            return val;
        }

        static constexpr int NUM_TYPES = 4 + amino_acid::NUM_AA * amino_acid::NUM_AA;

    private:
        explicit inline constexpr contact_type(int16_t val):
            val{val} {};

        int16_t val;

        friend class ::boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & val;
        }
    };
}