
#ifndef _GS_GRID_
#define _GS_GRID_

#include "index.hpp"
#include "concepts.hpp"

namespace gs{
/**
 * A grid of objects of arbitrary dimension.
 */
template<int N, class T, class S=uint32_t>
requires random_access_container<T> && iterable<T> && std::is_integral<S>::value
class grid {
    T m_storage;
    std::array<S, T> m_dimensions;

public:
    grid() = delete;
    grid(const std::array<S, T> dimensions): m_dimensions(dimensions) {}
    grid(const std::array<S, T> dimensions, const T storage): grid(dimensions), m_storage(storage) {}

    auto operator[](const index<N, S>& i) -> decltype(m_storage[i]) const {
        return m_storage[i.sub2ind(m_dimensions)];
    }
};
}

#endif