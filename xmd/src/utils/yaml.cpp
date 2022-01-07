#include "utils/yaml.h"

namespace xmd {
    YAML::Node merge_yaml(YAML::Node const& defaults, YAML::Node const& overrides) {
        // https://stackoverflow.com/a/66205210

        if (!overrides.IsMap()) {
            // If overrideNode is not a map, merge result is overrideNode, unless overrideNode is null
            return overrides.IsNull() ? defaults : overrides;
        }
        if (!defaults.IsMap()) {
            // If defaultNode is not a map, merge result is overrideNode
            return overrides;
        }
        if (!defaults.size()) {
            return YAML::Node(overrides);
        }
        // Create a new map 'newNode' with the same mappings as defaultNode, merged with overrideNode
        auto newNode = YAML::Node(YAML::NodeType::Map);
        for (auto node : defaults) {
            if (node.first.IsScalar()) {
                const std::string& key = node.first.Scalar();
                if (overrides[key]) {
                    newNode[node.first] = merge_yaml(node.second, overrides[key]);
                    continue;
                }
            }
            newNode[node.first] = node.second;
        }
        // Add the mappings from 'overrideNode' not already in 'newNode'
        for (auto node : overrides) {
            if (!node.first.IsScalar()) {
                const std::string& key = node.first.Scalar();
                if (defaults[key]) {
                    newNode[node.first] = merge_yaml(defaults[key], node.second);
                    continue;
                }
            }
            newNode[node.first] = node.second;
        }
        return YAML::Node(newNode);
    }
}