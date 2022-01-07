#pragma once
#include <cstdint>
#include <gentypes/gentype.h>

#define NAMESPACE(...) xmd,qa,__VA_ARGS__
#define TEMPLATE_PARAMS(...) __VA_ARGS__
#define NAME() sync_data
#define FIELDS() int8_t,back,int8_t,side_all,int8_t,side_polar,\
int8_t,side_hydrophobic

GEN_EXPR()

namespace xmd::qa {
    INST_NAME() {
        INST_BODY()
    public:
        sync_data() :
            back_{0}, side_all_{0}, side_polar_{0}, side_hydrophobic_{0} {};

        template<typename E>
        inline auto &operator+=(sync_data_expr<E> const &e) {
            back_ += e.back();
            side_all_ += e.side_all();
            side_polar_ += e.side_polar();
            side_hydrophobic_ += e.side_hydrophobic();
            return *this;
        }

        template<typename E>
        inline auto &operator-=(sync_data_expr<E> const &e) {
            back_ -= e.back();
            side_all_ -= e.side_all();
            side_polar_ -= e.side_polar();
            side_hydrophobic_ -= e.side_hydrophobic();
            return *this;
        }

        inline bool is_valid() const {
            return (back_ >= 0) && (side_all_ >= 0) && (side_polar_ >= 0) &&
                   (side_hydrophobic_ >= 0);
        }
    };
    INST_CONSTRUCT_DATA()

    REF_NAME() {
        REF_BODY()

    public:
        template<typename E>
        inline auto &operator+=(sync_data_expr<E> const &e) {
            back_ += e.back();
            side_all_ += e.side_all();
            side_polar_ += e.side_polar();
            side_hydrophobic_ += e.side_hydrophobic();
            return *this;
        }

        template<typename E>
        inline auto &operator-=(sync_data_expr<E> const &e) {
            back_ -= e.back();
            side_all_ -= e.side_all();
            side_polar_ -= e.side_polar();
            side_hydrophobic_ -= e.side_hydrophobic();
            return *this;
        }
    };
}

REF_IMPL_SPEC()

namespace xmd::qa {
    template<typename E1, typename E2>
    class sync_sum_expr:
        public sync_data_expr<sync_sum_expr<E1, E2>> {
    public:
        sync_sum_expr(E1 const& e1, E2 const& e2):
            e1{e1}, e2{e2} {};

        int8_t back() const {
            return e1.back() + e2.back();
        }

        int8_t side_all() const {
            return e1.side_all() + e2.side_all();
        }

        int8_t side_polar() const {
            return e1.side_polar() + e2.side_polar();
        }

        int8_t side_hydrophobic() const {
            return e1.side_hydrophobic() + e2.side_hydrophobic();
        }

    private:
        E1 e1;
        E2 e2;
    };

    template<typename E1, typename E2>
    inline auto operator+(sync_data_expr<E1> const& e1, sync_data_expr<E2> const& e2) {
        return sync_sum_expr<E1, E2>(static_cast<E1 const&>(e1),
            static_cast<E2 const&>(e2));
    }

    template<typename E1, typename E2>
    class sync_diff_expr:
        public sync_data_expr<sync_sum_expr<E1, E2>> {
    public:
        sync_diff_expr(E1 const& e1, E2 const& e2):
            e1{e1}, e2{e2} {};

        int8_t back() const {
            return e1.back() - e2.back();
        }

        int8_t side_all() const {
            return e1.side_all() - e2.side_all();
        }

        int8_t side_polar() const {
            return e1.side_polar() - e2.side_polar();
        }

        int8_t side_hydrophobic() const {
            return e1.side_hydrophobic() - e2.side_hydrophobic();
        }

    private:
        E1 e1;
        E2 e2;
    };

    template<typename E1, typename E2>
    inline auto operator-(sync_data_expr<E1> const& e1, sync_data_expr<E2> const& e2) {
        return sync_diff_expr<E1, E2>(static_cast<E1 const&>(e1),
            static_cast<E2 const&>(e2));
    }
}

GEN_CONST_REF()
GEN_PTR()
GEN_CONST_PTR()
GEN_SPAN()
GEN_CONST_SPAN()
GEN_MEMORY()
GEN_ALLOCATOR()
GEN_VECTOR()
GEN_SET()

#undef FIELDS
#undef NAME
#undef TEMPLATE_PARAMS
#undef NAMESPACE