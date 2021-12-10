#include "params/param_entry.h"

namespace xmd {
    void param_entry::load_from_file(const std::filesystem::path &file) {
        auto node = YAML::LoadFile(file);
        auto pwd = file.parent_path();
        load_from_node(node, pwd);
    }
}