#include "stats/gyration.h"

namespace xmd {
    void compute_gyration_radius::operator()() const {
        vec3r center_of_mass = vec3r::Zero();
        real total_mass = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            center_of_mass += mass[idx] * r[idx];
            total_mass += mass[idx];
        }
        center_of_mass /= total_mass;

        real gyration_r = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            gyration_r += norm_squared(r[idx] - center_of_mass);
        }

        *gyration_radius = sqrt(gyration_r / num_particles);
    }
}