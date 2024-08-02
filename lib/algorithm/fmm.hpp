
#ifndef _GS_FMM_
#define _GS_FMM_

#include "base/pattern.hpp"
#include "base/grid.hpp"

namespace gs {

/**
 * \brief A vector field.
 * 
 * The vector field provides the set of default template values for the 
 * fmm algorithm.
 */
template<int N, typename T> 
class vector_field {
public:
    using grid_val = std::pair<T, std::array<T, N>>;
    using box_val = std::array<T, N>;
    using point_list = std::array<grid_val, pow<2,N>()>;
    using f_box_weight = std::function<void(const point_list&, box_val&)>;
    using f_far_field = std::function<void(const box_val&, grid_val&)>;
    using f_near_field = std::function<void(const point_list&, grid_val&)>;
};

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
template<
    int N,
    typename T=uint32_t,
    class FFarField=vector_field<N, double>::f_far_field,
    class FNearField=vector_field<N, double>::f_near_field,
    class FBoxWeight=vector_field<N, double>::f_box_weight,
    class GridElement=vector_field<N, double>::grid_val,
    class BoxElement=vector_field<N, double>::box_val
>
requires (
    std::is_integral<T>::value &&
    std::invocable<FFarField&, const BoxElement&, GridElement&> &&
    std::invocable<FNearField&, const std::array<GridElement, pow<2,N>()>&, GridElement&> &&
    std::invocable<FBoxWeight&, const std::array<GridElement, pow<2,N>()>&, BoxElement&>
)
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
    void compute(T nIters=2){
        for (T it; it < nIters; ++it){
            m_grid.iterate(
                [&](box<N, T>& box, BoxElement& element, const PatternComponent pattern) {
                    switch (pattern) {
                        case COARSE_TO_FINE:
                            // Use the far field function for all levels apart
                            // from the most granular.
                            if (box.get_level() < m_grid.get_dimensions().max_level()){
                                for( const auto& corner : box){
                                    m_farFieldFunc(element, m_grid[corner]);
                                }
                            }
                            else {
                                for(const auto& corner : box){
                                    m_nearFieldFunc(m_grid.get_corner_values(box), m_grid[corner]);
                                }
                            }
                            break;
                        case FINE_TO_COARSE:
                            // Compute the box weights
                            m_boxWeightFunc(m_grid.get_corner_values(box), element);
                            break;
                        default:
                            break;
                    }
                },
                v_pattern()
            );
        }
    }

    GridElement& operator[](const index<N, T>& i){return m_grid[i];}
    const GridElement& operator[](const index<N, T>& i) const {return m_grid[i];}
    auto begin() -> decltype(m_grid.begin()){return m_grid.begin();}
    auto end() -> decltype(m_grid.end()){return m_grid.end();}
    auto begin() const -> decltype(m_grid.begin()){return m_grid.begin();}
    auto end() const -> decltype(m_grid.end()){return m_grid.end();}
};
}

#endif