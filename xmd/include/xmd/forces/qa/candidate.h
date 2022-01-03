#pragma once
#include <gentypes/gentype.h>
#include "contact_type.h"
#include "sync_data.h"

#define NAMESPACE(...) xmd,qa,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() candidate
#define FIELDS() int,i1,int,i2,int,free_pair_idx,xmd::qa::contact_type,type,\
xmd::qa::sync_data,sync_diff1,xmd::qa::sync_data,sync_diff2

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE