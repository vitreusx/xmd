#pragma once
#include <xmd/types/array.h>

namespace xmd {
    struct native_dihedral_span {
        array<int> i1, i2, i3, i4;
        array<real> nat_phi;
        int size;
    };
}