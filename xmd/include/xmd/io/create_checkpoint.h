#pragma once
#include <xmd/ctx/context.h>
#include <xmd/types/scalar.h>

namespace xmd {
    class create_checkpoint: public ctx_aware {
    public:
        std::string path_fmt;
        real period;

    public:
        context *def_ctx;
        real *last_t, *t;

        void declare_vars(context& ctx) override;

    public:
        void operator()() const;
    };
}