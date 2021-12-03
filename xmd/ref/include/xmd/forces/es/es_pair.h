#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/nl/nl_data.h>
#include <xmd/types/vec3.h>
#include <xmd/model/box.h>

namespace xmd {
    struct es_pair_span {
        array<int> i1, i2;
        array<float> q1_q2;
        int size;
    };

    struct es_pair_vector {
        vector<int> i1, i2;
        vector<float> q1_q2;
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

        inline auto to_span() const {
            es_pair_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.q1_q2 = q1_q2.data();
            s.size = size;
            return s;
        }
    };

    class update_es_pairs {
    public:
        float cutoff;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        es_pair_vector *pairs;

    public:
        void operator()() const;
    };
}