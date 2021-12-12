#pragma once
#include <xmd/types/vector.h>

namespace xmd {
    struct native_contact_span {
        array<int> i1, i2;
        int size;
    };

    class native_contact_vector {
    public:
        vector<int> i1, i2;
        int size;

        void sort();

        explicit native_contact_vector(int n = 0);
        native_contact_span to_span();
    };
}