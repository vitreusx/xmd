#pragma once
#include <boost/serialization/optional.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/split_free.hpp>
#include <yaml-cpp/yaml.h>

namespace boost::serialization {
    template<typename Archive>
    void save(Archive& ar, std::filesystem::path const& p, [[maybe_unused]] const unsigned int version) {
        ar << p.string();
    }

    template<typename Archive>
    void load(Archive& ar, std::filesystem::path& p, [[maybe_unused]] const unsigned int version) {
        std::string p_str;
        ar >> p_str;
        p = std::filesystem::path(p_str);
    }

    template<typename Archive>
    void serialize(Archive& ar, std::filesystem::path& p, const unsigned int version) {
        boost::serialization::split_free(ar, p, version);
    }

    template<typename Archive>
    void save(Archive& ar, YAML::Node const& node, [[maybe_unused]] const unsigned int version) {
        ar << YAML::Dump(node);
    }

    template<typename Archive>
    void load(Archive& ar, YAML::Node& node, [[maybe_unused]] const unsigned int version) {
        std::string node_text;
        ar >> node_text;
        node = YAML::Load(node_text);
    }

    template<typename Archive>
    void serialize(Archive& ar, YAML::Node& node, const unsigned int version) {
        boost::serialization::split_free(ar, node, version);
    }
}