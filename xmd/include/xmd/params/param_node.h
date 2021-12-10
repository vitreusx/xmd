#pragma once
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>

namespace xmd {
    class param_node {
    public:
        explicit param_node(std::filesystem::path const& file);

        std::filesystem::path resolve_path(
            std::filesystem::path const& relpath) const;

        template<typename T>
        T as() const {
            return node.as<T>();
        }

        param_node operator[](std::string const& tag) const;
        param_node operator[](int idx) const;

    private:
        YAML::Node node;
        std::filesystem::path location;

        explicit param_node(YAML::Node const& node,
            std::filesystem::path location);
    };
}