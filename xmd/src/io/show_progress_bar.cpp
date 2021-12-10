#include "io/show_progress_bar.h"
#include <iostream>
#include <chrono>
#include <xmd/utils/math.h>

namespace xmd {
    void show_progress_bar::operator()() const {
        using namespace std;
        using namespace std::chrono;

        true_real progress = *true_t / total_time;

        auto now = high_resolution_clock::now();
        auto diff = duration_cast<microseconds>(now - start_wall_time).count();

        cout << "\r" << "[";
        auto pos = (int)floor(width * progress);
        for (int i = 0; i < width; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << *true_t << " / " << total_time;
        cout << " V = " << *V << " E = "<< *E;
        cout << " t = " << diff/1000 << " ms";

        cout.flush();
    }

    void show_progress_bar::init_from_vm(vm &vm_inst) {
        true_t = &vm_inst.find<true_real>("true_t");
        V = &vm_inst.find<real>("V");
        E = &vm_inst.find<real>("E");
    }
}