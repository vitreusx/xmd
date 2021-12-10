#include "params/param_file.h"
#include "yaml_merge.h"

namespace xmd {
    param_file::param_file(const std::filesystem::path &location):
        param_file(YAML::LoadFile(location), location) {}

    param_file::param_file(const YAML::Node &yaml,
        const std::filesystem::path &location):
        param_entry(yaml, location) {};

    param_file param_value_parser<param_file>::parse(
        param_entry const& entry) const {

        YAML::Node yaml = entry;
        if (auto import_from = entry["import from"]; import_from) {
            for (auto const& relpath: import_from) {
                auto abspath = entry.location.value() / relpath.as<std::string>();
                auto imported = param_file(abspath);
                yaml = merge_nodes(yaml, imported);
            }
        }

        return param_file(yaml, entry.location.value());
    }
}