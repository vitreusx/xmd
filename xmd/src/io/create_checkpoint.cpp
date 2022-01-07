#include "io/create_checkpoint.h"
#include "params/yaml_fs_node.h"
#include "utils/text.h"
#include <fstream>

namespace xmd {
    void create_checkpoint::declare_vars(context &ctx) {
        auto& params = ctx.var<yaml_fs_node>("params");
        period = ctx.persistent<real>("checkpoint_period",
            params["checkpoints"]["exec period"].as<real>());
        path_fmt = ctx.persistent<std::string>("path_fmt",
            params["checkpoints"]["path format"].as<std::string>());

        def_ctx = &ctx;
        last_t = &ctx.ephemeral<real>("checkpoint_last_t",
            std::numeric_limits<real>::lowest());
        t = &ctx.var<real>("t");
    }

    void create_checkpoint::operator()() const {
        if (*t - *last_t > period) {
            using namespace std::filesystem;
            path fpath = format(path_fmt.c_str(), *t);
            create_directory(fpath.parent_path());
            std::ofstream checkpoint_file(fpath, std::ios::trunc);
            boost::archive::text_oarchive checkpoint_ar(checkpoint_file);
            checkpoint_ar << *def_ctx;
            *last_t = *t;
        }
    }
}