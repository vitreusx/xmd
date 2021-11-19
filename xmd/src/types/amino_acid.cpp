#include "amino_acid.h"

namespace xmd {
    std::vector<amino_acid> amino_acid::all() {
        std::vector<amino_acid> _all(amino_acid::NUM_AA);
        for (char idx = 0; idx < amino_acid::NUM_AA; ++idx) {
            _all[idx] = amino_acid((aa_code)idx);
        }
        return _all;
    }
}