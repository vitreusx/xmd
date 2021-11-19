#pragma once

namespace xmd::records {
    template<typename Record>
    Record *record::cast() {
        return std::get_if<Record>(&rec);
    }

    template<typename Record>
    Record const *record::cast() const {
        return std::get_if<Record>(&rec);
    }
}