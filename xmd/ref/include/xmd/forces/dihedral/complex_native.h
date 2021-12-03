#pragma once
#include <xmd/types/vec3.h>
#include "native_dihedral.h"

namespace xmd {
    class eval_cnd_forces {
    public:
        float CDA, CDB;

    public:
        vec3f_array r, F;
        native_dihedral_span dihedrals;
        float *V;

    public:
        void operator()() const;
    };
}
