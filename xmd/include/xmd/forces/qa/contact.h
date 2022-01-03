#pragma once
#include <xmd/types/vector.h>
#include <xmd/types/cyclic_buffer.h>
#include "contact_type.h"
#include "sync_data.h"

namespace xmd::qa {
    enum contact_status {
        FORMING_OR_FORMED, BREAKING
    };
}

#define NAMESPACE(...) xmd,qa,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() contact
#define FIELDS() int,i1,int,i2,contact_type,type,contact_status,status, \
real,ref_time,sync_data,sync_diff1,sync_data,sync_diff2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE