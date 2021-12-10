#pragma once
#include <yaml-cpp/yaml.h>

// https://stackoverflow.com/questions/41326112/how-to-merge-node-in-yaml-cpp
YAML::Node merge_nodes(YAML::Node const& a, YAML::Node const& b);