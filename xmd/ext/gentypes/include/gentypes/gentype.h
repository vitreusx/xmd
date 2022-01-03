#pragma once
#include "allocator.h"
#include "array.h"
#include "const_ptr.h"
#include "const_ref.h"
#include "const_span.h"
#include "expr.h"
#include "inst.h"
#include "memory.h"
#include "ptr.h"
#include "ref.h"
#include "set.h"
#include "span.h"
#include "vector.h"

#define GENTYPE()                                                              \
  GEN_EXPR()                                                                   \
  GEN_INST()                                                                   \
  GEN_REF()                                                                    \
  GEN_CONST_REF()                                                              \
  GEN_PTR()                                                                    \
  GEN_CONST_PTR()                                                              \
  GEN_CONST_SPAN()                                                             \
  GEN_SPAN()                                                                   \
  GEN_MEMORY()                                                                 \
  GEN_ALLOCATOR()                                                              \
  GEN_VECTOR()                                                                 \
  GEN_SET()