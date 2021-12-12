#include "model/native_contacts.h"
#include <xmd/model/model.h>
#include <algorithm>

namespace xmd {
    native_contact_vector::native_contact_vector(int n):
        i1{n}, i2{n}, size{n} {};

    native_contact_span native_contact_vector::to_span() {
        native_contact_span s;
        s.i1 = i1.to_array();
        s.i2 = i2.to_array();
        s.size = size;
        return s;
    }

    void native_contact_vector::sort() {
        std::vector<int> arg(size);
        std::iota(arg.begin(), arg.end(), 0);
        std::sort(arg.begin(), arg.end(), [&](int p, int q) -> bool {
            return std::make_pair(i1[p], i2[p]) < std::make_pair(i1[q], i2[q]);
        });

        vector<int> sorted_i1(size), sorted_i2(size);
        for (int idx = 0; idx < size; ++idx) {
            sorted_i1[idx] = i1[arg[idx]];
            sorted_i2[idx] = i2[arg[idx]];
        }

        i1 = sorted_i1;
        i2 = sorted_i2;
    }
}