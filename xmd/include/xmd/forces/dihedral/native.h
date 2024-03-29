#pragma once
#include <xmd/types/vec3.h>

#include <xmd/model/model.h>
#include <unordered_map>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nat_dih
#define FIELDS() int,i1,int,i2,int,i3,int,i4,xmd::real,nat_phi

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE
