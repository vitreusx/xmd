#pragma once
#include <xmd/types/array.h>
#include <xmd/types/vector.h>
#include <xmd/types/amino_acid.h>
#include <xmd/model/box.h>
#include <xmd/types/vec3.h>
#include <xmd/nl/nl_data.h>

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

        inline int push_back() {
            i1p.push_back();
            i1.push_back();
            i1n.push_back();
            i2p.push_back();
            i2.push_back();
            i2n.push_back();
            type.push_back();
            return size++;
        }

        inline void clear() {
            i1p.clear();
            i1.clear();
            i1n.clear();
            i2p.clear();
            i2.clear();
            i2n.clear();
            type.clear();
            size = 0;
        }

        inline auto to_span() const {
            pid_bundle_span s;
            s.i1p = i1p.data();
            s.i1 = i1.data();
            s.i1n = i1n.data();
            s.i2p = i2p.data();
            s.i2 = i2.data();
            s.i2n = i2n.data();
            s.size = size;
            return s;
        }
    };

    class update_pid_bundles {
    public:
        real cutoff;

    public:
        vec3r_array r;
        array<int> prev, next;
        array<amino_acid> atype;
        box<vec3r> *box;
        nl::nl_data *nl;
        pid_bundle_vector *bundles;

    public:
        void operator()() const;
    };
}