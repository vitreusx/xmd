#include "params/param_node.h"

namespace xmd {
    param_node::param_node(const std::filesystem::path &file) {
        node = YAML::LoadFile(file.string());
        location = file;
    }

    param_node param_node::operator[](const std::string &tag) const {
        return param_node(node[tag], location);
    }

    param_node param_node::operator[](int idx) const {
        return param_node(node[idx], location);
    }

    param_node::param_node(const YAML::Node &node,
        std::filesystem::path location):
        node {node}, location{std::move(location)} {}

    std::filesystem::path param_node::resolve_path(
            std::filesystem::path const& relpath) const {
        return location / relpath;
    };
}