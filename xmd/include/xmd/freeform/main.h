#pragma once
#include <xmd/store/store.h>

namespace xmd {
    class freeform_main {
    private:
        store st;

    public:
        int operator()(int argc, char **argv);
    };
}