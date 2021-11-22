#pragma once
#include <string>
#include <string_view>
#include <optional>
#include <vector>

namespace xmd::fields {
    class achar {
    public:
        explicit achar(size_t i);
        char read(std::string const& line) const;
        void write(std::string& line, char c) const;

    private:
        size_t i;
    };

    class atom {
    public:
        atom(size_t i, size_t j);
        std::string read(std::string const& line) const;
        void write(std::string& line, std::string const& atom) const;

    private:
        size_t beg, len;
    };

    class character {
    public:
        explicit character(size_t i);
        char read(std::string const& line) const;
        void write(std::string& line, char c) const;

    private:
        size_t i;
    };

//    class continuation {
//    public:
//        continuation(size_t i, size_t j);
//        size_t read(std::string const& line);
//        void write(std::string& line, size_t cont);
//    };
//
//    class date {
//    public:
//        using date_t = std::tuple<size_t, size_t, size_t>;
//        date(size_t i, size_t j);
//        date_t read(std::string const& line);
//        void write(std::string& line, date_t const& date);
//    };

    class integer {
    public:
        integer(size_t i, size_t j);
        int read(std::string const& line) const;
        void write(std::string& line, int n) const;

    private:
        size_t beg, len;
    };

    class lstring {
    public:
        lstring(size_t i, size_t j);
        std::string read(std::string const& line) const;
        void write(std::string& line, std::string const& ls) const;

    private:
        size_t beg, len;
    };

    class real {
    public:
        real(size_t i, size_t j, int n, int m);
        double read(std::string const& line) const;
        void write(std::string& line, double x) const;

    private:
        size_t beg, len;
        int n, m;
    };

    class record_name {
    public:
        explicit record_name(std::string const& name);
        bool read(std::string const& line) const;
        void write(std::string& line) const;

    private:
        std::string name;
    };

    class residue_name {
    public:
        residue_name(size_t i, size_t j);
        std::string read(std::string const& line) const;
        void write(std::string& line, std::string const& res_name) const;

    private:
        size_t beg, len;
    };

//    class slist {
//    public:
//
//    };

//    class specification {
//    public:
//
//    };

//    class specification_list {
//    public:
//
//    };

    class string {
    public:
        string(size_t i, size_t j);

        string& add(std::string const& line);
        std::string read() const;
        void clear();

        void write(std::vector<std::string*> const& lines,
            std::string const& text) const;

    private:
        size_t beg, len;
        std::string cur_text;
    };

    class literal {
    public:
        literal(size_t i, size_t j, std::string const& text);
        bool read(std::string const& line) const;
        void write(std::string& line) const;

    private:
        size_t beg, len;
        std::string text;
    };

//    class sym_op {
//    public:
//
//    };
}