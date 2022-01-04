#pragma once
#include "ptr.h"
#include "const_ptr.h"

template<typename T>
using array = ptr<T>;

template<typename T>
using const_array = const_ptr<T>;
