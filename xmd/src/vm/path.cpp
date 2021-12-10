#include "vm/path.h"

#include <utility>

namespace xmd {
    vm_path::vm_path():
        repr {""} {};

    vm_path::vm_path(const char *s):
        repr {std::string(s)} {};

    vm_path::vm_path(std::string s):
        repr{std::move(s)} {}

    vm_path vm_path::operator/(const vm_path &other) const {
        return vm_path(repr + "/" + other.repr);
    }

    bool vm_path::operator==(const vm_path &other) const {
        return repr == other.repr;
    }

    bool vm_path::operator!=(const vm_path &other) const {
        return repr != other.repr;
    }

}