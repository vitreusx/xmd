#include "params/param_file.h"
#include "yaml_merge.h"

namespace xmd {
    param_file::param_file(const std::filesystem::path &location):
        param_file(YAML::LoadFile(location), location) {}

    param_file::param_file(const YAML::Node &yaml,
        const std::filesystem::path &location):
        param_entry(yaml, location) {};

    param_file param_file::import(param_entry const& root,
        std::filesystem::path const& relpath) {

        auto def_location = std::filesystem::path();
        auto root_location = root.location.value_or(def_location);
        auto abspath = root_location.parent_path() / relpath;
        return param_file(abspath);
    }
}