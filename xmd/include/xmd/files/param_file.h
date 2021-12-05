#pragma once
#include <xmd/utils/param_entry.h>
#include "xmd/types/amino_acid.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <optional>
#include <array>

namespace xmd {
    class param_file {
    public:
        explicit param_file(std::filesystem::path const& pf_path);
        void load_entry(param_entry *entry);

    private:
        YAML::Node node;
        std::filesystem::path pwd;
        std::vector<param_file> inherited_pfs;
    };
}