#include "params/yaml_fs_node.h"
#include "utils/yaml.h"

namespace xmd {
    yaml_fs_node::yaml_fs_node(std::filesystem::path location):
        yaml_fs_node(YAML::LoadFile(location), std::move(location)) {};

    yaml_fs_node::yaml_fs_node(const YAML::Node &yaml, std::filesystem::path location):
        YAML::Node(yaml), location(std::move(location)) {};

    std::filesystem::path yaml_fs_node::resolve(const std::filesystem::path &path) const {
        return location / path;
    }

    yaml_fs_node yaml_fs_node::operator[](const std::string &key) const {
        return yaml_fs_node((*this).YAML::Node::operator[](key), location);
    }
}