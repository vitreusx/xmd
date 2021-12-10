#pragma once
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>
#include <optional>
#include <unordered_map>

namespace xmd {
    template<typename T>
    struct param_value_parser;

    class param_entry: public YAML::Node {
    public:
        explicit param_entry(YAML::Node const& yaml,
            std::optional<std::filesystem::path> location);

        template<typename T>
        T as() const;

        param_entry operator[](std::string const& tag) const;
        param_entry operator[](char const* tag) const;

    private:
        std::optional<std::filesystem::path> location;

        template<typename T>
        friend struct param_value_parser;
    };

    template<typename T>
    struct param_value_parser {
        T parse(param_entry const& entry) const {
            return entry.YAML::Node::as<T>();
        }
    };

    template<typename T>
    T param_entry::as() const {
        return param_value_parser<T>().parse(*this);
    }
}