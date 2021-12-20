#include "io/show_progress_bar.h"
#include <iostream>
#include <chrono>
#include <xmd/utils/math.h>
#include <xmd/params/param_file.h>
#include <xmd/utils/units.h>
#include <xmd/utils/text.h>

namespace xmd {
    std::string timedelta(uint64_t ms) {
        int ms_part = ms % 1000;
        uint64_t rem_s = (ms - ms_part) / 1000;
        int s_part = rem_s % 60;
        uint64_t rem_min = (rem_s - s_part) / 60;
        int min_part = rem_min % 60;
        uint64_t rem_hr = (rem_min - min_part) / 60;
        int hr_part = rem_hr % 24;
        int day_part = (rem_hr - hr_part) / 24;

        return format("%d:%02d:%02d:%02d.%04d",
            day_part, hr_part, min_part, s_part, ms_part);
    }

    void show_progress_bar::operator()() const {
        using namespace std;
        using namespace std::chrono;

        if (*first_time) {
            *start_t = *t;
            *last_t = std::numeric_limits<real>::lowest();
            *start_wall_time = high_resolution_clock::now();
            *first_time = false;
        }

        if (*t - *last_t > period) {
            auto progress = *t / total_time;

            auto now = high_resolution_clock::now();
            auto diff_ms = duration_cast<milliseconds>(now - *start_wall_time).count();

            cout << "\r" << "[";
            auto pos = (int)floor(width * progress);
            for (int i = 0; i < width; ++i) {
                if (i < pos) cout << "=";
                else if (i == pos) cout << ">";
                else cout << " ";
            }
            cout << "] " << *t << " / " << total_time;
            cout << " V = " << *V;
            cout << " t = " << timedelta(diff_ms);

            cout.flush();

            *last_t = *t;
        }
    }

    void show_progress_bar::init_from_vm(vm &vm_inst) {
        auto& params = vm_inst.find<param_file>("params");
        width = vm_inst.find_or_emplace<int>("pbar_width",
            params["progress bar"]["width"].as<int>());
        total_time = vm_inst.find_or_emplace<real>("total_time",
            params["general"]["total time"].as<quantity>());
        period = vm_inst.find_or_emplace<real>("pbar_period",
            params["progress bar"]["update period"].as<quantity>());

        t = &vm_inst.find<real>("t");
        V = &vm_inst.find<real>("V");
        first_time = &vm_inst.emplace<bool>("pbar_first_time", true);
        start_wall_time = &vm_inst.emplace<time_point_t>("start_wall_time");
        start_t = &vm_inst.emplace<real>("pbar_start_t");
        last_t = &vm_inst.emplace<real>("pbar_last_t");
    }
}