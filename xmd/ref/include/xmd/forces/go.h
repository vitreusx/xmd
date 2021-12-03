#pragma once
#include <xmd/types/vec3.h>
#include <xmd/types/vector.h>
#include <xmd/types/array.h>
#include <xmd/model/box.h>
#include <xmd/math.h>
#include <xmd/forces/primitives/lj.h>
#include <xmd/nl/nl_data.h>

namespace xmd {
    struct go_contact_span {
        array<int> i1, i2;
        array<float> nat_dist;
        int size;
    };

    struct go_contact_vector {
        vector<int> i1, i2;
        vector<float> nat_dist;
        int size;

        inline int push_back() {
            i1.push_back();
            i2.push_back();
            nat_dist.push_back();
            return size++;
        }

        inline void clear() {
            i1.clear();
            i2.clear();
            nat_dist.clear();
            size = 0;
        }

        inline auto to_span() const {
            go_contact_span s;
            s.i1 = i1.data();
            s.i2 = i2.data();
            s.nat_dist = nat_dist.data();
            s.size = size;
            return s;
        }
    };

    class update_go_contacts {
    public:
        float cutoff;

    public:
        vec3f_array r;
        box<vec3f> *box;
        nl::nl_data *nl;
        go_contact_vector *pairs;

    public:
        void operator()() const;
    };

    class eval_go_forces {
    public:
        float depth;

    public:
        vec3f_array r, F;
        box<vec3f> *box;
        go_contact_span contacts;
        float *V;

    public:
        void operator()() const;
    };
}