#include "nl/pair.h"

namespace xmd::nl {
    nl_pair_vector::nl_pair_vector(int n):
        i1{n}, i2{n}, is_native{n}, size{n} {};

    nl_pair_span nl_pair_vector::to_span() {
        nl_pair_span s;
        s.i1 = i1.to_array();
        s.i2 = i2.to_array();
        s.is_native = is_native.to_array();
        s.size = size;
        return s;
    }
}