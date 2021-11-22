#pragma once

namespace xmd {
    void compute_tether_forces::operator()() {
        for (size_t i = 0; i < pairs.size(); ++i) {
            const_ref<tether_pair> p = pairs[i];
            auto r1 = r[p.i1], r2 = r[p.i2];
            auto r12 = r2 - r1;

            auto r12_n = norm(r12);
            auto dx = r12_n - p.dist0;

            auto dx2 = dx * dx;
            *V += dx2 * (H1 + H2 * dx2);
            auto dV_dx = dx * (2.0f * H1 + 4.0f * H2 * dx2);

            auto r12_u = r12 / r12_n;
            auto f = dV_dx * r12_u;
            F[p.i1] -= f;
            F[p.i2] += f;
        }
    }
}