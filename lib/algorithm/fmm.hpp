
#ifndef _GS_FMM_
#define _GS_FMM_

#include "base/pattern.hpp"
#include "base/grid.hpp"
#include "math/polynomial.hpp"

namespace gs {

template<int N, typename T, class FTraversal, class FBoxWeight,class GridElement,class BoxElement>
requires (
    std::is_integral<T>::value &&
    std::invocable<FTraversal&, const box<N, T>&, const BoxElement&, grid<N, GridElement, BoxElement, T>&> &&
    std::invocable<FBoxWeight&, const box<N, T>&, BoxElement&, const grid<N, GridElement, BoxElement, T>&>
)
/**
 * \brief The Fast Multipole Method.
 * 
 * The Fast Multipole Method is a method which arose from electrostatics.
 * The principle is to enable fast computation of problems which require 
 * comparing each point in the grid to each point, leading to at least O(N^2)
 * complexity.
 * 
 * Assuming that the function which is used to compare points is analytic, the
 * principle is to use Taylor's theorem to approximate the function when comparing
 * points which are distant from each other, and only compute the precise value 
 * for nearby points. The algorithm splits the grid in to a 2^N tree (a quad-tree in 2D,
 * and an oct-tree in 3D), and parses the tree top-to-bottom and back up again computing 
 * the solution.
 * 
 * In the most general sense, the algorithm computes matrix multiplication on a grid,
 * for matrices which are generated from analytic functions such as covariance functions
 * and smoothed derivatives. It can also be used to invert a matrix as well.
 * 
 * The template parameters,
 *      N           - The number of dimensions of the underlying grid.
 *      T           - The base integral type.
 *      FTraversal  - The function used to traverse each of the finest grid boxes.
 *      FBoxWeight  - A function which takes a list of points in a box and produces a
 *                    box value, to be used as a weight in the far field equation.
 *      GridElement - The element type to be stored at each point in the grid.
 *      BoxElement  - The element type to be stored at each box in the tree.
 */
class fmm {
    grid<N, GridElement, BoxElement, T> m_grid; ///< The underlying grid.
    FTraversal m_fineTraversalFunc; ///< The traversal function for the fines level
    FBoxWeight m_boxWeightFunc; ///< The box weight function.

public:
    fmm(
        const dimensions<N, T> dimensions,
        FTraversal fineTraversalFunc,
        FBoxWeight boxWeightFunc
    ) :
        m_grid(dimensions),
        m_fineTraversalFunc(fineTraversalFunc),
        m_boxWeightFunc(boxWeightFunc){}

    /**
     * \brief Compute the solution.
     * 
     * The algorithm iterates each level of the tree in coarse-to-fine and
     * then at each of the finest nodes, reach back up to the coarsest levels
     * to get the multipoles. In the fine-to-coarse traversal, the weights
     * to each of the boxes are computed and stored.
     */
    void compute(const T nIters=1){
        for (T it = 0; it < nIters; ++it){
            m_grid.iterate(
                [&](box<N, T>& box, BoxElement& element, const PatternComponent pattern) {
                    switch (pattern) {
                        case PARSE_FINEST:
                            // Depth first at the finest nodes.
                            m_fineTraversalFunc(box, element, m_grid);
                            break;
                        case FINE_TO_COARSE:
                            // Compute the box weights
                            m_boxWeightFunc(box, element, m_grid);
                            break;
                        default:
                            break;
                    }
                },
                {FINE_TO_COARSE, PARSE_FINEST}
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