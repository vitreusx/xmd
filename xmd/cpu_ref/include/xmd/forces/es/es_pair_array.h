#pragma once
#include <xmd/types/array.h>

namespace xmd {
    struct es_pair_span {
        array<int> i1, i2;
        array<float> q1_q2;
        int size;
    };
}