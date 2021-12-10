#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/nl/data.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>
#include <xmd/vm/vm.h>

namespace xmd {
    struct es_pair_span {
        array<int> i1, i2;
        array<real> q1_q2;
        int size;
    };

    struct es_pair_vector {
        vector<int> i1, i2;
        vector<real> q1_q2;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            q1_q2.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            q1_q2.clear();
            size = 0;
        }

        es_pair_span to_span();
    };

    class update_es_pairs: public vm_aware {
    public:
        real cutoff;

    public:
        vec3r_array r;
        box<vec3r> *box;
        nl::nl_data *nl;
        es_pair_vector *pairs;

        void init_from_vm(vm& vm_inst) override;

    public:
        void operator()() const;
    };
}