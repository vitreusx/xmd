#include "forces/qa/sync_data.h"

namespace xmd::qa {
    sync_data_vector::sync_data_vector(int n):
        back(n), side_all(n), side_polar(n), side_hydrophobic(n),
        size_(n) {};
}