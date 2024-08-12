
#ifndef _GS_MUTLIGRID_
#define _GS_MUTLIGRID_

#include "base/pattern.hpp"
#include "base/grid.hpp"
#include "math/polynomial.hpp"

namespace gs {
template<int N,typename T,class FBoxFunc,class GridElement,class BoxElement>
requires (
    std::is_integral<T>::value &&
    std::invocable<FBoxFunc&, const box<N, T>&, const BoxElement&, grid<N, GridElement, BoxElement, T>&, const PatternComponent>
)
/**
 * \brief The multigrid method
 */
class fmm {
    grid<N, GridElement, BoxElement, T> m_grid; ///< The underlying grid.
    FBoxFunc m_boxFunc; ///< The box function.

public:
    fmm(
        const dimensions<N, T> dimensions,
        FFarField boxFunc,
    ) : m_grid(dimensions), m_boxFunc(boxFunc) {}

    /**
     * \brief Compute the solution.
     * 
     * The algorithm iterates each level of the tree in coarse-to-fine and
     * fine-to-coarse direction. In the fine-to-coarse traversal, the weights
     * to each of the boxes are computed and stored. In the coarse-to-fine 
     * traversal the corners of the box are used to compute the solution using
     * the far-field equation in all of the levels apart from the finest, 
     * the near field equation is used at the finest level.
     */
    void compute(const T nIters=1){
        for (T it = 0; it < nIters; ++it){
            m_grid.iterate(
                [&](box<N, T>& box, BoxElement& element, const PatternComponent pattern) {
                    m_boxFunc(box, element, m_grid, pattern);
                },
                inverse_v_pattern()
            );
        }
    }

    size_t grid_size() const{return m_grid.size();} ///< Get the number of vertices in the grid
    GridElement& operator[](const T i){return m_grid[i];} ///< Access the ith vertex of the grid
    const GridElement& operator[](const T i) const {return m_grid[i];} ///< Access the ith vertex of the grid
    GridElement& operator[](const index<N, T>& i){return m_grid[i];} ///< Access a vertex of the grid using an index
    const GridElement& operator[](const index<N, T>& i) const {return m_grid[i];} ///< Access a vertex of the grid using an index
    auto begin() -> decltype(m_grid.begin()){return m_grid.begin();} ///< Return a begin iterator into the vertices
    auto end() -> decltype(m_grid.end()){return m_grid.end();} ///< Return the end iterator into the vertices
    auto begin() const -> decltype(m_grid.begin()){return m_grid.begin();} ///< Return a begin iterator into the vertices
    auto end() const -> decltype(m_grid.end()){return m_grid.end();} ///< Return the end iterator into the vertices
};
}

#endif