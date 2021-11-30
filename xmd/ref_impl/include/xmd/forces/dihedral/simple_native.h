#pragma once
#include <xmd/types/vec3.h>
#include "native_dihedral.h"

namespace xmd {
    class eval_snd_forces {
    public:
        float CDH;

    public:
        vec3f_array r, F;
        native_dihedral_span dihedrals;
        float *V;

    public:
        void operator()() const;
    };
}
