#pragma once
#include <istream>
#include <string_view>
#include <iterator>
#include <vector>
#include <optional>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace xmd {
    class csv_record;
    class csv_header;
    class csv_file;

    class csv_record {
    public:
        explicit csv_record(std::vector<std::string> fields);

        std::string const& operator[](size_t col_idx) const;
        std::string const& operator[](std::string const& col_name) const;

        friend std::ostream& operator<<(std::ostream& os,
            csv_record const& record);

        std::vector<std::string> fields;

    private:
        friend class csv_file;
        friend class csv_header;

        csv_record() = default;
        explicit csv_record(const std::string& line,
            csv_header *header = nullptr);

        csv_header* header;
    };

    class csv_header {
    public:
        csv_header() = default;
        explicit csv_header(std::vector<std::string> fields);

        std::string const& operator[](size_t idx) const;
        size_t operator[](std::string const& s) const;

        friend std::ostream& operator<<(std::ostream& os,
            csv_header const& header);

    private:
        friend class csv_file;
        explicit csv_header(csv_record&& record);

        csv_record header_record;
        std::unordered_map<std::string, size_t> name_to_idx;
    };

    class csv_file {
    public:
        csv_file() = default;
        explicit csv_file(std::istream&& file, bool header = true);
        explicit csv_file(std::string const& text, bool header = true);

        std::ostream& print(std::ostream& os, bool header = true);
        std::string print(bool header = true);

        csv_file& operator<<(csv_record record);

        std::optional<csv_header> header;
        std::vector<csv_record> records;
    };
}
