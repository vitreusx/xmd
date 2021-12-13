#pragma once
#include <istream>
#include <string_view>
#include <iterator>
#include <vector>
#include <optional>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <memory>
#include <xmd/params/param_entry.h>

namespace xmd {
    class csv_record;
    class csv_header;
    class csv_file;

    class csv_record {
    public:
        explicit csv_record(std::vector<std::string> fields);
        explicit csv_record(std::shared_ptr<csv_header> header = nullptr);

        explicit csv_record(const std::string& line,
            std::shared_ptr<csv_header> header = nullptr);

        std::string const& operator[](size_t col_idx) const;
        std::string& operator[](size_t col_idx);

        std::string const& operator[](std::string const& col_name) const;
        std::string& operator[](std::string const& col_name);

        friend std::ostream& operator<<(std::ostream& os,
            csv_record const& record);

        std::vector<std::string> fields;

        std::shared_ptr<csv_header> header;
    };

    class csv_header {
    public:
        csv_header() = default;
        explicit csv_header(std::vector<std::string> fields);

        std::string const& operator[](size_t idx) const;
        size_t num_columns() const;
        size_t operator[](std::string const& s) const;

        friend std::ostream& operator<<(std::ostream& os,
            csv_header const& header);

        explicit csv_header(csv_record&& record);

        csv_record header_record;
        std::unordered_map<std::string, size_t> name_to_idx;
    };

    class csv_file {
    public:
        csv_file() = default;
        explicit csv_file(std::istream&& file, bool header = true);
        explicit csv_file(std::string const& text, bool header = true);

        std::ostream& print(std::ostream& os, bool header = true) const;
        std::string print(bool header = true) const;

        csv_record& add_record();

        std::shared_ptr<csv_header> header;
        std::vector<csv_record> records;

        void set_header(std::vector<std::string> col_names);
    };

    template<>
    struct param_value_parser<csv_file> {
        csv_file parse(param_entry const& entry) const;
    };
}

namespace YAML {
    template<>
    struct convert<xmd::csv_file> {
        static Node encode(const xmd::csv_file& csv);
        static bool decode(const Node& node, xmd::csv_file& csv);
    };
}