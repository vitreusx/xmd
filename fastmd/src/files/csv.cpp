#include "csv.h"
using namespace fastmd;

std::string csv_record::operator[](size_t idx) const {
    return fields[idx];
}

size_t csv_record::size() const {
    return fields.size();
}

csv_record::csv_record(csv_file& _csv, std::string const& line) {
    csv = &_csv;

    char const *start = line.c_str();
    for (auto const& c: line) {
        if (c == ',') {
            auto slice = std::string_view(start, &c - start);
            fields.push_back((std::string)slice);
            start = &c + 1;
        }
    }

    char const* final = line.c_str() + line.size();
    auto final_slice = std::string_view(start, final - start);
    fields.emplace_back((std::string)final_slice);
}

auto csv_record::begin() {
    return fields.begin();
}

auto csv_record::end() {
    return fields.end();
}

std::string csv_record::operator[](const std::string &s) const {
    if (csv->header) {
        return (std::string)fields[csv->header.value()[s]];
    }
    else {
        throw;
    }
}

csv_record const& csv_iterator::operator*() const {
    return record;
}

csv_record const* csv_iterator::operator->() const {
    return &record;
}

csv_iterator& csv_iterator::operator++() {
    pos = next;
    fetch_record();
    return *this;
}

csv_iterator csv_iterator::operator++(int) {
    auto sav = *this;
    ++*this;
    return sav;
}

bool fastmd::operator==(const csv_iterator &iter1, const csv_iterator &iter2) {
    return iter1.pos == iter2.pos;
}

bool fastmd::operator!=(const csv_iterator &iter1, const csv_iterator &iter2) {
    return !(iter1 == iter2);
}

void csv_iterator::fetch_record() {
    csv.file.seekg(pos, std::ios_base::beg);

    if (!csv.file.eof()) {
        std::string line;
        std::getline(csv.file, line); csv.file.clear();
        next = csv.file.tellg();
        record = csv_record(csv, std::move(line));
    }
}

csv_iterator::csv_iterator(csv_file &csv): csv{csv} {
    pos = csv.file.tellg();
    fetch_record();
}

csv_file::csv_file(std::istream &file, bool has_header): file{file} {
    if (has_header) {
        auto iter = csv_iterator(*this);
        header = csv_header(std::move(iter.record));
        field_start_pos = iter.next;
    }
    else {
        field_start_pos = file.tellg();
    }
}

csv_iterator csv_file::begin() {
    auto pos = file.tellg();
    file.seekg(field_start_pos, std::ios_base::beg);
    auto iter = csv_iterator(*this);
    file.seekg(pos);
    return iter;
}

csv_iterator csv_file::end() {
    auto pos = file.tellg();
    file.seekg(0, std::ios_base::end);
    auto iter = csv_iterator(*this);
    file.seekg(pos);
    return iter;
}

size_t csv_header::operator[](const std::string &s) {
    return name_to_idx.at(s);
}

auto csv_header::begin() {
    return record.begin();
}

csv_header::csv_header(csv_record&& record_): record{std::move(record_)} {
    for (size_t col_idx = 0; col_idx < record.size(); ++col_idx) {
        name_to_idx[(std::string)record[col_idx]] = col_idx;
    }
}
