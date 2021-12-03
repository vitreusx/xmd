#pragma once
#include <xmd/forces/primitives/harmonic.h>
#include <xmd/model/box.h>
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/nl/nl_data.h>
#include <xmd/types/vec3.h>

namespace xmd {
    struct nat_ssbond_span {
        array<int> i1, i2;
        int size;
    };

    struct nat_ssbond_vector {
        vector<int> i1, i2;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            size = 0;
        }

        inline auto to_span() const {
            nat_ssbond_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.size = size;
            return s;
        }
    };

    class update_nat_ssbonds {
    public:
        float cutoff;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        nat_ssbond_vector *pairs;

    public:
        void operator()() const;
    };

    class eval_nat_ssbond_forces {
    public:
        float H1, nat_r;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        nat_ssbond_span bonds;
        float *V;

    public:
        void operator()() const;
    };
}