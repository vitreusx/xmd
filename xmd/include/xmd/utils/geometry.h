#pragma once
#include <xmd/types/vec3.h>

namespace xmd {
    namespace v3 {
        template<typename U>
        class aff3 {
        public:
            U a_xx, a_xy, a_xz, t_x;
            U a_yx, a_yy, a_yz, t_y;
            U a_zx, a_zy, a_zz, t_z;
        };

        template<typename U, typename E>
        class apply_aff_expr: public expr<apply_aff_expr<U, E>> {
        public:
            apply_aff_expr(aff3<U> const& aff, E const& e):
                aff{aff}, e{e} {};

            auto x() {
                return aff.a_xx * e.x() + aff.a_xy * e.y()
                    + aff.a_xz * e.z() + aff.t_x;
            }

            auto y() {
                return aff.a_yx * e.x() + aff.a_yy * e.y()
                    + aff.a_yz * e.z() + aff.t_y;
            }

            auto z() {
                return aff.a_zx * e.x() + aff.a_zy * e.y()
                    + aff.a_zz * e.z() + aff.t_z;
            }

        private:
            aff3<U> const& aff;
            E const& e;
        };

        template<typename U>
        class plane {
        public:
            vec<U> origin, normal;
        };

        template<typename E, typename U>
        auto signed_dist(expr<E> const& point, plane<U> const& plane) {
            return dot(plane.normal, point - plane.origin);
        }

        template<typename E, typename U>
        auto project(expr<E> const& point, plane<U> const& plane) {
            return point - plane.normal * signed_dist(point, plane);
        }
    }

    using aff3f = v3::aff3<float>;
    using plane = v3::plane<float>;
}