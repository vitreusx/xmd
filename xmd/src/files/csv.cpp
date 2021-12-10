#include "files/csv.h"
#include <fstream>

namespace xmd {
    csv_record::csv_record(std::vector<std::string> fields):
        fields{std::move(fields)} {};

    std::string const&csv_record::operator[](size_t col_idx) const {
        return fields[col_idx];
    }

    std::string const &
    csv_record::operator[](const std::string &col_name) const {
        return fields[(*header)[col_name]];
    }

    // https://stackoverflow.com/a/30338543
    enum class CSVState {
        UnquotedField,
        QuotedField,
        QuotedQuote
    };

    std::vector<std::string> readCSVRow(const std::string &row) {
        CSVState state = CSVState::UnquotedField;
        std::vector<std::string> fields {""};
        size_t i = 0; // index of the current field
        for (char c : row) {
            switch (state) {
            case CSVState::UnquotedField:
                switch (c) {
                case ',': // end of field
                    fields.push_back(""); i++;
                    break;
                case '"': state = CSVState::QuotedField;
                    break;
                default:  fields[i].push_back(c);
                    break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                case '"': state = CSVState::QuotedQuote;
                    break;
                default:  fields[i].push_back(c);
                    break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                case ',': // , after closing quote
                    fields.push_back(""); i++;
                    state = CSVState::UnquotedField;
                    break;
                case '"': // "" -> "
                    fields[i].push_back('"');
                    state = CSVState::QuotedField;
                    break;
                default:  // end of quote
                    state = CSVState::UnquotedField;
                    break; }
                break;
            }
        }
        return fields;
    }

    csv_record::csv_record(const std::string &line,
        std::shared_ptr<csv_header> header) {

        fields = readCSVRow(line);
        this->header = std::move(header);
    }

    std::ostream& operator<<(std::ostream& os, csv_record const& record) {
        for (size_t i = 0; i < record.fields.size(); ++i) {
            os << record[i];
            if (i+1 < record.fields.size()) os << ",";
        }
        return os;
    }

    std::string const& csv_header::operator[](size_t idx) const {
        return header_record[idx];
    }

    size_t csv_header::operator[](const std::string &s) const {
        return name_to_idx.at(s);
    }

    std::ostream& operator<<(std::ostream& os, csv_header const& header) {
        os << header.header_record;
        return os;
    }

    csv_header::csv_header(std::vector<std::string> fields):
        csv_header(csv_record(std::move(fields))) {};

    csv_header::csv_header(csv_record &&record) {
        header_record = std::move(record);
        for (size_t i = 0; i < header_record.fields.size(); ++i) {
            name_to_idx[header_record[i]] = i;
        }
    }

    csv_file::csv_file(std::istream &&file, bool header) {
        for (std::string line; std::getline(file, line); ) {
            if (header) {
                auto _header = csv_header(csv_record(line));
                this->header = std::make_shared<csv_header>(_header);
                header = false;
            }
            else {
                records.push_back(csv_record(line, this->header));
            }
        }
    }

    csv_file::csv_file(const std::string &text, bool header):
        csv_file(std::stringstream(text), header) {};

    std::ostream& csv_file::print(std::ostream& os, bool header) {
        if (header && this->header)
            os << *this->header << '\n';

        for (auto const& record: records)
            os << record << '\n';

        return os;
    }

    std::string csv_file::print(bool header) {
        std::stringstream ss {};
        print(ss, header);
        return ss.str();
    }

    csv_file& csv_file::operator<<(csv_record record) {
        records.emplace_back(std::move(record));
        return *this;
    }

    void csv_file::set_header(std::vector<std::string> col_names) {
        auto _header = csv_header(std::move(col_names));
        header = std::make_shared<csv_header>(_header);
    }

    csv_file param_value_parser<csv_file>::parse(
        const param_entry &entry) const {

        if (auto from_file = entry["from file"]; from_file) {
            auto file = std::ifstream(from_file.as<std::string>());
            return csv_file(std::move(file));
        }
        else {
            auto source_txt = entry.as<std::string>();
            return csv_file(source_txt);
        }
    }
}
