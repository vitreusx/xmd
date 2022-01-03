#include "stats/rmsd.h"
#include <xmd/utils/convert.h>
#include <Eigen/Core>
#include <Eigen/SVD>

namespace xmd {
    void compute_rmsd::init_from_vm(vm& vm_inst) {
        rmsd = &vm_inst.find<real>("rmsd");
        r = vm_inst.find<vector<vec3r>>("r").data();
        ref_r = vm_inst.find_or_emplace<vector<vec3r>>("rmsd_ref_r").data();
        num_particles = vm_inst.find<int>("num_particles");

        for (int idx = 0; idx < num_particles; ++idx) {
            ref_r[idx] = r[idx];
        }
    }

    void compute_rmsd::operator()() const {
        using matrix_t = Eigen::Matrix<real, Eigen::Dynamic, 3>;
        matrix_t P = matrix_t::Zero(num_particles, 3);
        matrix_t Q = matrix_t::Zero(num_particles, 3);

        for (int idx = 0; idx < num_particles; ++idx) {
            P.row(idx) = convert<real>(r[idx]);
            Q.row(idx) = convert<real>(ref_r[idx]);
        }

        Eigen::Matrix<real, 3, 3> H = P.transpose() * Q;
        auto svd = Eigen::JacobiSVD<decltype(H)>(H,
            Eigen::ComputeFullU & Eigen::ComputeFullV);

        auto U = svd.matrixU();
        auto V = svd.matrixV();
        auto d = (V * U.transpose()).determinant() > 0 ? 1 : -1;
        auto R = V * Eigen::DiagonalMatrix<real, 3>(1, 1, d) * U.transpose();

        auto D = (P - Q) * R;
        real rmsd_ = 0.0;
        for (int idx = 0; idx < num_particles; ++idx) {
            rmsd_ += D.row(idx).squaredNorm();
        }
        *rmsd = sqrt(rmsd_ / num_particles);
    }
}