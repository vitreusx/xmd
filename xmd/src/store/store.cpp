#include "store/store.h"

namespace xmd {
    bool temp_var::has_value() const {
        return value_ptr != nullptr;
    }

    bool saved_var::has_value() const {
        return value_ptr != nullptr;
    }

    store& store::per_thread() {

    }
}