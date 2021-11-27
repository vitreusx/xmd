#pragma once

namespace xmd {
    struct native_dihedral_array {
        int *i1, *i2, *i3, *i4;
        float *nat_phi;
        int size;
    };
}