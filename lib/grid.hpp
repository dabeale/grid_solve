
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
requires random_access_container<T> && resizable<T> && std::is_integral<S>::value
class grid {
    T m_storage;
    dimensions<N, S> m_dimensions;

public:
    grid() = delete;
    grid(const dimensions<N, S> dimensions): m_dimensions(dimensions) {}
    grid(const dimensions<N, S> dimensions, T&& storage): m_dimensions(dimensions), m_storage(storage) {
        const size_t maxInd = dimensions.max_ind(dimensions.max_level());
        if(storage.size() != maxInd){
            storage.resize(maxInd);
        }
    }

    typename T::value_type& operator()(const index<N, S>& index) {
        return m_storage[
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        ];
    }

    const typename T::value_type& operator() (const index<N, S>& index) const {
        return m_storage[
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        ];
    }

    /**
     * Iterate over every box at the specified level.
     */
    template<class F>
    requires std::invocable<F&, box<N, S>&>
    void iterate(
        const F& callable,
        const S level
    ){
        const S max_ind = m_dimensions.max_ind(level);
        for(S i=0; i<max_ind; ++i){
            box<N, S> box(m_dimensions, level, i);
            callable(box);
        }
    }

    /**
     * A means to traverse the grid.
     */
    enum PatternComponent {
        FINE_TO_COARSE,
        COARSE_TO_FINE
    };
    /**
     * Iterate over every box at the specified level.
     */
    template<class F>
    requires std::invocable<F&, box<N, S>&>
    void iterate(
        const F& callable,
        std::vector<PatternComponent> pattern
    ){
        const S max_level = m_dimensions.max_level();
        for(const auto pattern : pattern){
            switch(pattern){
                case grid<N,T,S>::FINE_TO_COARSE:
                    for(S i = 0; i <= max_level; ++i){
                        grid<N,T,S>::iterate(callable, i);
                    }
                    break;
                case grid<N,T,S>::COARSE_TO_FINE:
                    for(S i = 0; i <= max_level; ++i){
                        grid<N,T,S>::iterate(callable, max_level-i);
                    }
                    break;
                default:
                    break;
            }
        }
    }
};
}

#endif