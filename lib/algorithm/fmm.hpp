
#ifndef _GS_FMM_
#define _GS_FMM_

#include "base/pattern.hpp"
#include "base/grid.hpp"
#include "math/polynomial.hpp"

namespace gs {

template<int N, typename T, int D, typename S=uint32_t> 
/**
 * \brief A vector field.
 * 
 * The vector field provides the set of default template values for the 
 * fmm algorithm.
 * 
 * The template parameters,
 *      N           - The number of dimensions of the underlying grid.
 *      T           - The unit type (e.g. double)
 *      D           - The approximation degree.
 */
class vector_field {
public:
    using grid_val = std::tuple<T, T, vector<T, N>, int32_t>;
    using box_val = std::pair<polynomial<T, N, D>, vector<T, N>>;
    using f_box_weight = std::function<void(const box<N, S>&, box_val&, const grid<N, grid_val, box_val, S>&)>;
    using f_far_field = std::function<void(const box<N, S>&, const box_val&, grid<N, grid_val, box_val, S>&)>;
    using f_near_field = std::function<void(const box<N, S>&, grid<N, grid_val, box_val, S>&)>;
};

template<
    int N,
    typename T=uint32_t,
    class FFarField=vector_field<N, double, 2>::f_far_field,
    class FNearField=vector_field<N, double, 2>::f_near_field,
    class FBoxWeight=vector_field<N, double, 2>::f_box_weight,
    class GridElement=vector_field<N, double, 2>::grid_val,
    class BoxElement=vector_field<N, double, 2>::box_val
>
requires (
    std::is_integral<T>::value &&
    std::invocable<FFarField&, const box<N, T>&, const BoxElement&, grid<N, GridElement, BoxElement, T>&> &&
    std::invocable<FNearField&, const box<N, T>&, grid<N, GridElement, BoxElement, T>&> &&
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
 *      FFarField   - The far field equation which takes a pre-computed value
 *                    in each box, and produces an grid value for each input point.
 *      FNearField  - The near field equation which takes a list of points (at corners 
 *                    of granular boxes), and produces an output value for each point.
 *      FBoxWeight  - A function which takes a list of points in a box and produces a
 *                    box value, to be used as a weight in the far field equation.
 *      GridElement - The element type to be stored at each point in the grid.
 *      BoxElement  - The element type to be stored at each box in the tree.
 */
class fmm {
    grid<N, GridElement, BoxElement, T> m_grid; ///< The underlying grid.
    FFarField m_farFieldFunc; ///< The far field function.
    FNearField m_nearFieldFunc; ///< The near field function.
    FBoxWeight m_boxWeightFunc; ///< The box weight function.

public:
    fmm(
        const dimensions<N, T> dimensions,
        FFarField farFieldFunc,
        FNearField nearFieldFunc,
        FBoxWeight boxWeightFunc
    ) :
        m_grid(dimensions),
        m_farFieldFunc(farFieldFunc),
        m_nearFieldFunc(nearFieldFunc),
        m_boxWeightFunc(boxWeightFunc){}

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
                    switch (pattern) {
                        case COARSE_TO_FINE:
                            // Use the far field function for all levels apart
                            // from the most granular.
                            if (box.get_level() < m_grid.get_dimensions().max_level()){
                                m_farFieldFunc(box, element, m_grid);
                            }
                            else {
                                m_nearFieldFunc(box, m_grid);
                            }
                            break;
                        case FINE_TO_COARSE:
                            // Compute the box weights
                            m_boxWeightFunc(box, element, m_grid);
                            break;
                        default:
                            break;
                    }
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