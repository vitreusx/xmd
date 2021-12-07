#include "vm/path.h"

#include <utility>

namespace xmd {
    vm_path::vm_path():
        path {""} {};

    vm_path::vm_path(const char *s):
        path {std::string(s)} {};

    vm_path::vm_path(std::string s):
        path{std::move(s)} {}

    vm_path vm_path::operator/(const vm_path &other) const {
        return vm_path(path + "/" + other.path);
    }

    bool vm_path::operator==(const vm_path &other) const {
        return path == other.path;
    }

    bool vm_path::operator!=(const vm_path &other) const {
        return path != other.path;
    }

}