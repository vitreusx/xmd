#include "vm/vm.h"

namespace xmd {
    resource_not_found::resource_not_found(vm_path const& path):
        std::logic_error("Resource \"" + path.repr + "\" has not been added/emplaced to the VM.") {};
}