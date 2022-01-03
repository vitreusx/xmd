#pragma once
#include <gentypes/gentype.h>
#include <xmd/types/scalar.h>

#define NAMESPACE(...) xmd,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nat_cont
#define FIELDS() int,i1,int,i2,xmd::real,nat_dist

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE