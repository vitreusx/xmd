#include "params/param_entry.h"

namespace xmd {
    param_entry param_entry::operator[](const std::string &tag) const {
        return param_entry(YAML::Node::operator[](tag), location);
    }

    param_entry param_entry::operator[](char const* tag) const {
        return param_entry(YAML::Node::operator[](tag), location);
    }

    param_entry param_entry::operator[](int idx) const {
        return param_entry(YAML::Node::operator[](idx), location);
    }

    param_entry::param_entry(const YAML::Node &yaml,
        std::optional<std::filesystem::path> location):
        YAML::Node{yaml}, location{std::move(location)} {};
}