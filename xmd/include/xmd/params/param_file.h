#pragma once
#include "param_entry.h"
#include <filesystem>

namespace xmd {
    class param_file {
    public:
        explicit param_file(std::filesystem::path const& location);
        explicit param_file(YAML::Node const& yaml,
            std::filesystem::path const& location);

        operator param_entry const&() const;

    private:
        param_entry entry;
        friend struct param_value_parser<param_file>;
    };

    template<>
    struct param_value_parser<param_file> {
        param_file parse(param_entry const& entry) const;
    };
}