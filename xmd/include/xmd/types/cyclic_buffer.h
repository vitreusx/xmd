#pragma once
#include "vector.h"

namespace xmd {
    template<typename T>
    class cyclic_buffer {
    public:
        T& push(T const& item) {
            if ((int)buf.size() == size()) {
                int old_size = buf.size();
                if (old_size > 0) start %= old_size;

                int new_size = (int)ceil(1.5f * (float)old_size) + 32;
                buf.resize(new_size);
                std::move(buf.data(), buf.data() + start,
                    buf.data() + old_size);
                end = start + old_size;
            }

            return (buf[(end++) % buf.size()] = item);
        }

        T pop() {
            return std::move(buf[(start++) % buf.size()]);
        }

        int size() const {
            return end - start;
        }

        bool empty() const {
            return start >= end;
        }

        void clear() {
            buf.clear();
            start = end = 0;
        }

    private:
        vector<T> buf;
        int start, end;
    };
}