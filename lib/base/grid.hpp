
#ifndef _GS_GRID_
#define _GS_GRID_

#include <functional>

#include "base/dimensions.hpp"
#include "base/box.hpp"
#include "base/index.hpp"
#include "base/concepts.hpp"
#include "base/pattern.hpp"

namespace gs{
template<int N, class GridElement, class BoxElement, class S=uint32_t>
requires std::is_integral<S>::value
/**
 * \brief A grid of objects of arbitrary dimension.
 * 
 * The grid is a heap-allocated, contiguous N-dimensional tensor, 
 * which can be used for computing the solutions to analytic equations 
 * defined on the grid.
 * 
 * The grid is defined by an arbitrary number of dimensions, of arbitrary
 * size. An object is stored at each point, and can be traversed point-by-point
 * but also at any particular level in its associated 2^n-tree. The tree levels
 * area accessed using a box, and objects can be stored and retrieved
 * at each level.
 * 
 * The tree is parsed using the iterate method, which will iterate all of
 * the points using the specified pattern. For example, in the fmm algorithm
 * it is parsed from the lowest level to the top and back again.
 * 
 *  * The template parameters,
 *      M           - The number of dimensions of the grid.
 *      GridElement - The object to store at each point.
 *      BoxElement  - The object to store at each level.
 *      S           - The integral type to use.
 */
class grid {
    std::vector<GridElement> m_gridStorage; ///< Storage at each of the points in the grid.
    std::vector<std::vector<BoxElement>> m_boxStorage; ///< Storage at each level of the 2^N tree.
    dimensions<N, S> m_dimensions; ///< The dimensions of the grid.

public:
    grid() = delete;
    grid(const dimensions<N, S> dimensions):
        m_gridStorage(dimensions.max_ind(dimensions.max_level())),
        m_boxStorage(dimensions.max_level()+1),
        m_dimensions(dimensions)
    {
        S i=0;
        for(auto& boxStore : m_boxStorage){
            boxStore.resize(dimensions.reduce().max_ind(i++));
        }
    }

    /**
     * \brief Set the grid storage.
     */
    void set_grid(const std::vector<GridElement>& grid){
        m_gridStorage = grid;
    }

    /**
     * \brief Get the size of the grid.
     */
    size_t size() const { return m_gridStorage.size(); }

    /**
     * \brief Get the dimensions of the grid.
     */
    const dimensions<N, S>& get_dimensions() const {
        return m_dimensions;
    }

    /**
     * \brief Access the grid storage using an integer.
     */
    GridElement& operator[](const S i) {
        return m_gridStorage[i];
    }
    /**
     * \brief Access the grid storage using an integer.
     */
    const GridElement& operator[](const S i) const {
        return m_gridStorage[i];
    }

    /**
     * \brief Access the grid storage using an index.
     */
    GridElement& operator[](const index<N, S>& index) {
        return operator[](
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        );
    }

    /**
     * \brief Access the grid storage using an index.
     */
    const GridElement& operator[] (const index<N, S>& index) const {
        return operator[](
            m_dimensions.sub2ind(
                index.at_level(m_dimensions.max_level()),
                m_dimensions.max_level()
            )
        );
    }

    /**
     * \brief Get the corner values of the box in terms of
     * the grid storage object.
     */
    std::array<GridElement, pow<2,N>()> get_corner_values(const box<N, S>& box) const{
        std::array<GridElement, pow<2,N>()> gridValues;
        S iCorner=0;
        for(const auto& corner : box){
            gridValues[iCorner++] = grid<N, GridElement, BoxElement, S>::operator[](corner);
        }
        return gridValues;
    }

    auto begin() -> decltype(m_gridStorage.begin()){return m_gridStorage.begin();} ///< Return a begin iterator into the vertices
    auto end() -> decltype(m_gridStorage.end()){return m_gridStorage.end();} ///< Return the end iterator into the vertices
    auto begin() const -> decltype(m_gridStorage.begin()){return m_gridStorage.begin();} ///< Return a begin iterator into the vertices
    auto end() const -> decltype(m_gridStorage.end()){return m_gridStorage.end();} ///< Return the end iterator into the vertices

    template<class F>
    requires std::invocable<F&, box<N, S>&, BoxElement&>
    /**
     * \brief Iterate over every box at the specified level.
     */
    void iterate(
        const F& callable,
        const S level
    ){
        const S max_ind = m_dimensions.reduce().max_ind(level);
        for(S i=0; i<max_ind; ++i){
            box<N, S> box(m_dimensions, level, i);
            callable(box, m_boxStorage[level][i]);
        }
    }

    template<class F>
    requires std::invocable<F&, box<N, S>&, BoxElement&, PatternComponent>
    /**
     * \brief Iterate over every box at the specified level.
     */
    void iterate(
        const F& callable,
        const std::vector<PatternComponent>& pattern
    ){
        const S max_level = m_dimensions.max_level();
        for(const auto pattern : pattern){
            switch(pattern){
                case COARSE_TO_FINE:
                    for(S i = 0; i <= max_level; ++i){
                        grid<N,GridElement,BoxElement,S>::iterate(
                            [&](box<N,S>& box, BoxElement& element){
                                callable(box, element, pattern);
                            }, i
                        );
                    }
                    break;
                case FINE_TO_COARSE:
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