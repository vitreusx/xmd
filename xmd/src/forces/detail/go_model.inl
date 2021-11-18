#pragma once

namespace xmd {
    void compute_go_model::operator()() {
        for (size_t idx = 0; idx < contacts.size(); ++idx) {
            const_ref<go_contact> cont = contacts[idx];
            auto r1 = r[cont.i1], r2 = r[cont.i2];
            auto r12 = box->uv(r1, r2);
            auto r12_rn = rnorm(r12);
            auto r12_u = r12 * r12_rn;

            auto s = r12_rn * cont.native_dist;
            auto s6 = s * s * s * s * s * s, s12 = s6 * s6;
            *V = depth * (s12 - 2.0f * s6);

            auto dV_dr = 12.0f * depth * r12_rn * (s6 - s12);
            F[cont.i1] -= r12_u * dV_dr;
            F[cont.i2] += r12_u * dV_dr;
        }
    }
}