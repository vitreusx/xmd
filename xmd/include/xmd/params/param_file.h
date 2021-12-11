#pragma once
#include "param_entry.h"
#include <filesystem>

namespace xmd {
    class param_file: public param_entry {
    public:
        explicit param_file(std::filesystem::path const& location);
        param_file(YAML::Node const& yaml,
            std::filesystem::path const& location);

        static param_file import(param_entry const& root,
            std::filesystem::path const& relpath);

    private:
        friend struct param_value_parser<param_file>;
    };
}