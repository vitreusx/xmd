#pragma once
#include <yaml-cpp/yaml.h>

namespace xmd {
    YAML::Node merge_yaml(YAML::Node const& defaults, YAML::Node const& overrides);
}