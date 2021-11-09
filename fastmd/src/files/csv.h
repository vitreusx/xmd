#pragma once
#include <istream>
#include <string_view>
#include <iterator>
#include <vector>
#include <optional>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace fastmd {
    class csv_record;
    class csv_iterator;
    class csv_file;

    class csv_record {
    public:
        std::string operator[](size_t idx) const;
        std::string operator[](std::string const& s) const;
        [[nodiscard]] size_t size() const;

        auto begin();
        auto end();

    private:
        friend class csv_iterator;
        csv_record() = default;
        explicit csv_record(csv_file& csv, std::string const& line);

        csv_file* csv;
        std::vector<std::string> fields;
    };

    class csv_iterator:
        public std::iterator<std::forward_iterator_tag, const csv_record> {
    public:
        csv_record const& operator*() const;
        csv_record const* operator->() const;

        csv_iterator& operator++();
        csv_iterator operator++(int);

        friend bool operator==(csv_iterator const& iter1, csv_iterator const& iter2);
        friend bool operator!=(csv_iterator const& iter1, csv_iterator const& iter2);

    private:
        friend class csv_file;
        explicit csv_iterator(csv_file& csv);
        void fetch_record();

        csv_file& csv;
        std::istream::pos_type pos, next;
        csv_record record;
    };

    class csv_header {
    public:
        size_t operator[](std::string const& s);
        auto begin();
        auto end();

    private:
        friend class csv_file;
        explicit csv_header(csv_record&& record);

        csv_record record;
        std::unordered_map<std::string, size_t> name_to_idx;
    };

    class csv_file {
    public:
        csv_file(std::istream& file, bool has_header = true);
        csv_iterator begin();
        csv_iterator end();

        std::optional<csv_header> header;

    private:
        friend class csv_iterator;
        std::istream& file;
        std::istream::pos_type field_start_pos;
    };
}
