#include "fields.h"
#include <algorithm>
#include <regex>

static std::string lstrip(std::string const& s) {
    auto beg = std::find_if_not(s.begin(), s.end(), [](auto c) -> auto {
        return std::isspace(c);
    });
    auto end = s.end();
    return s.substr(beg - s.begin(), end - beg);
}

static std::string rstrip(std::string const& s) {
    auto beg = s.begin();
    auto end = std::find_if_not(s.rbegin(), s.rend(), [](auto c) -> auto {
        return std::isspace(c);
    }).base();
    return s.substr(beg - s.begin(), end - beg);
}

static std::string strip(std::string const& s) {
    return rstrip(lstrip(s));
}

template<typename... Args>
std::string format(const char *fmt, Args const&... args) {
    int req_len = snprintf(nullptr, 0, fmt, args...);
    std::string s(req_len, '\0');
    snprintf(s.data(), req_len, fmt, args...);
    return s;
}

namespace xmd::fields {
    achar::achar(size_t i):
        i{i-1} {}

    char achar::read(const std::string &line) const {
        return line[i];
    }

    void achar::write(std::string &line, char c) const {
        line[i] = c;
    };

    atom::atom(size_t i, size_t j):
        beg{i-1}, len{j-i+1} {}

    std::string atom::read(const std::string &line) const {
        return strip(line.substr(beg, len));
    }

    void atom::write(std::string &line, std::string const& atom) const {
        auto cut_atom = format("%*s", len, atom.c_str());
        std::string_view(line.data() + beg, len) = cut_atom;
    }

    character::character(size_t i):
        i{i-1} {};

    char character::read(const std::string &line) const {
        return line[i];
    }

    void character::write(std::string &line, char c) const {
        line[i] = c;
    }

    integer::integer(size_t i, size_t j):
        beg{i-1}, len{j-i+1} {};

    int integer::read(const std::string &line) const {
        return std::stoi(line.substr(beg, len));
    }

    void integer::write(std::string &line, int n) const {
        auto cut_n = format("%*d", len, n);
        std::string_view(line.data() + beg, len) = cut_n;
    }

    lstring::lstring(size_t i, size_t j, int n):
        beg{i-1}, len{j-i+1} {}

    std::string lstring::read(const std::string &line) const {
        return line.substr(beg, len);
    }

    void lstring::write(std::string &line, std::string const& ls) const {
        std::string_view(line.data() + beg, len) = ls;
    }

    real::real(size_t i, size_t j, int n, int m):
        beg{i-1}, len{j-i+1}, n{n}, m{m} {}

    double real::read(const std::string &line) const {
        return std::stod(line.substr(beg, len));
    }

    void real::write(std::string &line, double x) const {
        auto x_str = format("%*.*f", n, m, x);
        auto cut_x_str = format("%*s", len, x_str.c_str());
        std::string_view(line.data() + beg, len) = cut_x_str;
    }

    record_name::record_name(const std::string &name):
        name{name} {}

    bool record_name::read(const std::string &line) const {
        auto header = lstring(1, 6).read(line);
        return header == name;
    }

    void record_name::write(std::string &line) const {
        lstring(1, 6).write(line, name);
    }

    residue_name::residue_name(size_t i, size_t j):
        beg{i-1}, len{j-i+1} {}

    std::string residue_name::read(const std::string &line) const {
        return strip(line.substr(beg, len));
    }

    void
    residue_name::write(std::string &line, std::string const& res_name) const {
        auto cut_res_name = format("%*s", len, res_name.c_str());
        std::string_view(line.data() + beg, len) = cut_res_name;
    }

    string::string(size_t i, size_t j, int n):
        beg{i-1}, len{j-i+1} {}

    string &string::add(const std::string &line) {
        cur_text += line;
        return *this;
    }

    std::string string::read() const {
        return strip(std::regex_replace(cur_text, std::regex("\\s+"), " "));
    };

    void string::clear() {
        cur_text = "";
    }

    void string::write(const std::vector<std::string *> &lines,
        const std::string &text) const {

        for (size_t line_idx = 0, text_idx = 0; text_idx < text.size();
        ++line_idx, text_idx += len) {
            auto& line = *lines[line_idx];
            std::string_view(line.data() + beg, len) = text.substr(text_idx, len);
        }
    }

    literal::literal(size_t i, size_t j, std::string const& text):
        beg{i-1}, len{j-i+1}, text{text} {};

    bool literal::read(const std::string &line) const {
        auto line_text = line.substr(beg, len);
        return line_text == text;
    }

    void literal::write(std::string& line) const {
        std::string_view(line.data() + beg, len) = text;
    }
}