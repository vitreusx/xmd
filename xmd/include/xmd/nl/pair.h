#pragma once


#include <xmd/types/scalar.h>

#define NAMESPACE(...) xmd,nl,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() nl_pair
#define FIELDS() int,i1,int,i2,bool,is_native

GENTYPE()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE