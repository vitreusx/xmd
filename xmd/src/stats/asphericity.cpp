#include "stats/asphericity.h"
#include <Eigen/Core>
#include <Eigen/SVD>

namespace xmd {
    void compute_asphericity::init_from_vm(vm &vm_inst) {
        asphericity = &vm_inst.emplace<real>("asphericity");
        r = vm_inst.find<vec3r_vector>("r").to_array();
        mass = vm_inst.find<vector<real>>("mass").to_array();
        num_particles = vm_inst.find<int>("num_particles");
    }

    static Eigen::Vector3<real> cast(vec3r const& v) {
        return { v.x(), v.y(), v.z() };
    }

    void compute_asphericity::operator()() const {
        vec3r center_of_mass = vec3r::Zero();
        real total_mass = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            center_of_mass += mass[idx] * r[idx];
        }
        center_of_mass /= total_mass;

        using matrix_t = Eigen::Matrix<real, 3, Eigen::Dynamic>;
        matrix_t R = matrix_t::Zero(3, num_particles);
        for (int idx = 0; idx < num_particles; ++idx) {
            R.col(idx) = cast(r[idx] - center_of_mass);
        }

        auto lambda = Eigen::JacobiSVD<decltype(R)>(R).singularValues();
        *asphericity = (real)1.5 * pow(lambda.z(), 2.0)
            - (real)0.5 * lambda.squaredNorm();
    }
}