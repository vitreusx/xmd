#pragma once
#include <vector>

namespace xmd {
    template<typename T>
    class cyclic_buffer {
    public:
        T& push(T const& item) {
            if (buf.size() == size()) {
                auto old_size = buf.size();
                start %= old_size;

                buf.resize(2*old_size);
                std::move(buf.begin(), buf.begin() + start,
                    buf.begin() + old_size);
                end = start + old_size;
            }

            return (buf[(end++) % buf.size()] = item);
        }

        T pop() {
            return std::move(buf[(start++) % buf.size()]);
        }

        size_t size() const {
            return end - start;
        }

        bool empty() const {
            return start >= end;
        }

    private:
        std::vector<T> buf;
        size_t start, end;
    };
}