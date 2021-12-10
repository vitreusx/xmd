#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/types/amino_acid.h>
#include <xmd/model/box.h>
#include <xmd/types/vec3.h>
#include <xmd/nl/data.h>
#include <xmd/vm/vm.h>

namespace xmd::pid {
    struct pid_bundle_span {
        array<int> i1p, i1, i1n;
        array<int> i2p, i2, i2n;
        array<int16_t> type;
        int size;
    };

    class pid_bundle_vector {
    public:
        vector<int> i1p, i1, i1n;
        vector<int> i2p, i2, i2n;
        vector<int16_t> type;
        int size;

        int push_back();
        void clear();
        pid_bundle_span to_span();
    };

    class update_pid_bundles: public vm_aware {
    public:
        real cutoff;

    public:
        vec3r_array r;
        array<int> prev, next;
        array<amino_acid> atype;
        box<vec3r> *box;
        nl::nl_data *nl;
        pid_bundle_vector *bundles;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}