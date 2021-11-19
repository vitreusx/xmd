#pragma once
#include <iostream>
#include <optional>
#include <variant>
#include <Eigen/Core>

namespace xmd::records {
    class remark {
    public:
        int number;
        std::string text;

    public:
        static std::optional<remark> try_parse(std::string const& line);
        std::string write() const;
    };

    class atom {
    public:
        int serial;
        std::string atom_name;
        std::string residue_name;
        char chain_id;
        int res_seq_num;
        Eigen::Vector3d pos;

    public:
        static std::optional<atom> try_parse(std::string const& line);
        std::string write() const;
    };

    class ssbond {
    public:
        int serial;
        struct {
            char chain_id;
            int res_seq_num;
        } res[2];
        double length;

    public:
        static std::optional<ssbond> try_parse(std::string const& line);
        std::string write() const;
    };

    class link {
    public:
        struct {
            std::string atom_name;
            std::string res_name;
            char chain_id;
            int res_seq_num;
        } res[2];
        double length;

    public:
        static std::optional<link> try_parse(std::string const& line);
        std::string write() const;
    };

    class cryst1 {
    public:
        Eigen::Vector3d cell;

    public:
        static std::optional<cryst1> try_parse(std::string const& line);
        std::string write() const;
    };

    class ter {
    public:
        int serial;
        std::string res_name;
        char chain_id;
        int res_seq_num;

    public:
        static std::optional<ter> try_parse(std::string const& line);
        std::string write() const;
    };

    class model {
    public:
        int serial;

    public:
        static std::optional<model> try_parse(std::string const& line);
        std::string write() const;
    };

    class endmdl {
    public:
        static std::optional<endmdl> try_parse(std::string const& line);
        std::string write() const;
    };

    class end {
    public:
        static std::optional<end> try_parse(std::string const& line);
        std::string write() const;
    };

    class record {
    public:
        using record_variant_t = std::variant<remark, atom,
            ssbond, link, cryst1, ter, model, endmdl, end>;
        record_variant_t rec;

    public:
        static std::optional<record> try_parse(std::string const& line);
        std::string write() const;

        template<typename Record>
        Record *cast();

        template<typename Record>
        Record const *cast() const;

    private:
        explicit record(record_variant_t rec);
    };
}

#include "records.inl"
