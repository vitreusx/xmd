#pragma once
#include <xmd/types/vec3.h>
#include "native_dihedral.h"

namespace xmd {
    class eval_snd_forces {
    public:
        real CDH;

    public:
        vec3r_array r, F;
        native_dihedral_span dihedrals;
        real *V;

    public:
        void operator()() const;
    };
}
