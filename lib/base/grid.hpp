
#ifndef _GS_GRID_
#define _GS_GRID_

#include <functional>

#include "base/dimensions.hpp"
#include "base/box.hpp"
#include "base/index.hpp"
#include "base/concepts.hpp"
#include "base/pattern.hpp"

namespace gs{
/**
 * A grid of objects of arbitrary dimension.
 */
template<int N, class GridElement, class BoxElement, class S=uint32_t>
requires std::is_integral<S>::value
class grid {
    std::vector<GridElement> m_gridStorage;
    std::vector<std::vector<BoxElement>> m_boxStorage;
    dimensions<N, S> m_dimensions;

public:
    grid() = delete;
    grid(const dimensions<N, S> dimensions):
        m_dimensions(dimensions),
        m_gridStorage(dimensions.max_ind(dimensions.max_level())),
        m_boxStorage(dimensions.max_level()+1) 
    {
        S i=0;
        for(auto& boxStore : m_boxStorage){
            boxStore.resize(dimensions.max_ind(i++));
        }
    }

    /**
     * Get the dimensions of the grid.
     */
    const dimensions<N, S>& get_dimensions() const {
        return m_dimensions;
    }

    GridElement& operator[](const index<N, S>& index) {
        return m_gridStorage[
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        ];
    }

    const GridElement& operator[] (const index<N, S>& index) const {
        return m_gridStorage[
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        ];
    }

    /**
     * Get the corner values of the box in terms of
     * the grid storage object.
     */
    std::array<GridElement, pow<2,N>()> get_corner_values(const box<N, S>& box){
        std::array<GridElement, pow<2,N>()> gridValues;
        S iCorner=0;
        for(const auto& corner : box){
            gridValues[iCorner++] = grid<N, GridElement, BoxElement, S>::operator[](corner);
        }
        return gridValues;
    }

    auto begin() -> decltype(m_gridStorage.begin()){return m_gridStorage.begin();}
    auto end() -> decltype(m_gridStorage.end()){return m_gridStorage.end();}
    auto begin() const -> decltype(m_gridStorage.begin()){return m_gridStorage.begin();}
    auto end() const -> decltype(m_gridStorage.end()){return m_gridStorage.end();}

    /**
     * Iterate over every box at the specified level.
     */
    template<class F>
    requires std::invocable<F&, box<N, S>&, BoxElement&>
    void iterate(
        const F& callable,
        const S level
    ){
        const S max_ind = m_dimensions.max_ind(level);
        for(S i=0; i<max_ind; ++i){
            box<N, S> box(m_dimensions, level, i);
            callable(box, m_boxStorage[level][i]);
        }
    }

    /**
     * Iterate over every box at the specified level.
     */
    template<class F>
    requires std::invocable<F&, box<N, S>&, BoxElement&, PatternComponent>
    void iterate(
        const F& callable,
        const std::vector<PatternComponent>& pattern
    ){
        const S max_level = m_dimensions.max_level();
        for(const auto pattern : pattern){
            switch(pattern){
                case grid<N,GridElement,BoxElement,S>::FINE_TO_COARSE:
                    for(S i = 0; i <= max_level; ++i){
                        grid<N,GridElement,BoxElement,S>::iterate(
                            [&](box<N,S>& box, BoxElement& element){
                                callable(box, element, pattern);
                            }, i
                        );
                    }
                    break;
                case grid<N,GridElement,BoxElement,S>::COARSE_TO_FINE:
                    for(S i = 0; i <= max_level; ++i){
                        grid<N,GridElement,BoxElement,S>::iterate(
                            [&](box<N,S>& box, BoxElement& element){
                                callable(box, element, pattern);
                            }, max_level-i
                        );
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