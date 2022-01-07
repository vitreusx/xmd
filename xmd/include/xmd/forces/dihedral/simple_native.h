#pragma once
#include <xmd/types/vec3.h>
#include "native.h"

namespace xmd {
    class eval_snd_forces:
        public eval_native_dihedral_forces_base {
    public:
        real CDH;

        void declare_vars(context& ctx) override;

    public:
        void iter(int idx) const;
        void operator()() const;
        void omp_async() const;
    };
}
