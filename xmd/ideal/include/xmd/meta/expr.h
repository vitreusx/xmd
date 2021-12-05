#pragma once
#include <xmd/meta/macros.h>

#define FIELD_FN(type, name) \
    auto name() const {\
        return static_cast<E const&>(*this).name();                          \
    }

#define EXPR_TYPE(name, fields) \
    template<typename E> \
    class name##_expr {\
    public:                     \
        \
    };