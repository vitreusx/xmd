#pragma once
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace xmd {
    class yaml_fs_node;

    template<typename T>
    struct yaml_fs_value_parser {
        static T parse(yaml_fs_node const& node);
    };

    class yaml_fs_node: public YAML::Node {
    public:
        yaml_fs_node() = default;
        explicit yaml_fs_node(std::filesystem::path location);
        explicit yaml_fs_node(YAML::Node const& yaml, std::filesystem::path location);

        std::filesystem::path resolve(std::filesystem::path const& path) const;
        yaml_fs_node operator[](std::string const& key) const;

        template<typename T>
        T as() const {
            return yaml_fs_value_parser<T>::parse(*this);
        }

    private:
        std::filesystem::path location;

        friend class ::boost::serialization::access;

        template<typename Archive>
        void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
            ar & ::boost::serialization::base_object<YAML::Node>(*this);
            ar & location;
        }
    };

    template<typename T>
    T yaml_fs_value_parser<T>::parse(const yaml_fs_node &node) {
        return node.YAML::Node::as<T>();
    }
}
