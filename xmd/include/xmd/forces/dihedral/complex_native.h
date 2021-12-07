#pragma once
#include <xmd/types/vec3.h>
#include "native.h"

namespace xmd {
    class eval_cnd_forces:
        public eval_native_dihedral_forces_base {
    public:
        real CDA, CDB;

    public:
        void operator()() const;
    };
}
