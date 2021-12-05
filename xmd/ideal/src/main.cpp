#include <iostream>
#include <xmd/meta/macros.h>

#define NAME(type, name) name
#define TYPE(type, name) type

template<typename E>
class expr_ {
public:
#define FIELD_EXPR(type, name) \
    auto name() const {\
        return static_cast<E const&>(*this).name();                           \
    };
#define FIELD_EXPR_(acc, x) acc FIELD_EXPR x
#define FIELD_EXPR_LIST(...) FOLDL(FIELD_EXPR_, , __VA_ARGS__)
};

#define GEN_EXPR(name, ...) \
    template<typename E>      \
    class name##_expr {            \
    public:                 \
        FIELD_EXPR_LIST(__VA_ARGS__)                           \
    };

GEN_EXPR(pid_bundle, (int, i1p), (int, i1), (int, i1n),
    (int, i2p), (int, i2), (int, i2n))

int main() {
    return 0;
}