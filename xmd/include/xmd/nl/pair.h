#pragma once


#include <xmd/types/scalar.h>

#define NAMESPACE(...) xmd,nl,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nl_pair
#define FIELDS() int,i1,int,i2,xmd::real,orig_dist

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE

namespace xmd::nl {
    inline int compare(nl_pair const& p, nl_pair const& q) {
        if (p.i1() < q.i1()) return -1;
        else if (p.i1() > q.i1()) return 1;
        else if (p.i2() < q.i2()) return -1;
        else if (p.i2() > q.i2()) return 1;
        else return 0;
    }

    inline bool operator<(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) < 0;
    }

    inline bool operator<=(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) <= 0;
    }

    inline bool operator>(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) > 0;
    }

    inline bool operator>=(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) >= 0;
    }

    inline bool operator==(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) == 0;
    }

    inline bool operator!=(nl_pair const& p, nl_pair const& q) {
        return compare(p, q) != 0;
    }
}