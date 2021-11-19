#pragma once

namespace xmd::records {
    template<typename Record>
    Record *record::as() {
        return std::get_if<Record>(&rec);
    }

    template<typename Record>
    Record const *record::as() const {
        return std::get_if<Record>(&rec);
    }
}