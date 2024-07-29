
#ifndef _GS_GRID_
#define _GS_GRID_

#include <functional>

#include "dimensions.hpp"
#include "box.hpp"
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
    dimensions<N, S> m_dimensions;

public:
    grid() = delete;
    grid(const dimensions<N, S> dimensions): m_dimensions(dimensions) {}
    grid(const dimensions<N, S> dimensions, const T storage): m_dimensions(dimensions), m_storage(storage) {}

    void iterate(
        std::function< void(box<N, S>&) > lambda,
        const S level
    ){
        const S max_ind = m_dimensions.max_ind(level);
        for(S i=0; i<max_ind; ++i){
            box<N, S> box(m_dimensions, level, i);
            lambda(box);
        }
    }
};
}

#endif