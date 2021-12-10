#pragma once
#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace xmd {
    class param_entry {
    public:
        virtual void load_from_node(YAML::Node const& root,
            std::filesystem::path const& pwd) = 0;

        void load_from_file(std::filesystem::path const& file);
    };
}