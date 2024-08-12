
#ifndef _GS_ANALYTIC_MULTIPLY_
#define _GS_ANALYTIC_MULTIPLY_

#include "algorithm/fmm.hpp"
#include "functions/exp_squared.hpp"

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
    using f_traversal = std::function<void(const box<N, S>&, const box_val&, grid<N, grid_val, box_val, S>&)>;
};

template<typename T, size_t M, size_t D, template < typename, size_t, size_t > class FuncEstimator>
requires (
    (M > 0) && (D > 0) &&
    requires(FuncEstimator<T, M, D> t) {
        { t(vector<T, M>(), vector<T, M>()) } -> std::same_as<T>;
        { t.estimate(polynomial<T, M, D>(), vector<T, M>(), vector<T, M>()) } -> std::same_as<T>;
        { 
            t.compute_coefs(std::array<vector<T, M>, 1>(), vector<T, M>(), std::array<T, 1>()) 
        } -> std::constructible_from<polynomial<T, M, D>>;
    }
)
/**
 * \brief An approximation of matrix multiplication, when the
 * matrix is generated by an analytic function.
 */
class analytic_multiply  {
    static constexpr size_t nBoxCorners = pow<2,M>(); ///< The number of corners of each box.

    using f_traversal = vector_field<M, T, D>::f_traversal; ///< The traversal function
    using f_box_weight = vector_field<M, T, D>::f_box_weight; ///< The box weight function
    using grid_val = gs::vector_field<M,T,D>::grid_val; ///< A value in the grid
    using box_val = gs::vector_field<M,T,D>::box_val;   ///< A value stored in each box
    using box_corners = std::array<vector<T,M>, nBoxCorners>; ///< The corners of the box
    using box_values = std::array<T, nBoxCorners>;      ///< The values at each corner

    /**
     * \brief Get the values and points at the corners of 
     * the input box.
     */
    static std::pair<box_corners, box_values> corner_vals(
        const gs::box<M>& boxImp,
        const gs::grid<M, grid_val, box_val>& grid
    ) {
        std::pair<box_corners, box_values> ret;
        for(size_t i=0; i<analytic_multiply::nBoxCorners; ++i){
            ret.first[i] = std::get<2>(grid[boxImp[i]]);
            ret.second[i] = std::get<1>(grid[boxImp[i]]);
        }
        return ret;
    };

    dimensions<M> m_dimensions; ///< The dimensions.
    FuncEstimator<T,M,D> m_f_estimator; ///< The analytic function estimator.
    fmm<M, uint32_t, f_traversal, f_box_weight, grid_val, box_val> m_fmm; ///< The FMM method.

public:
    analytic_multiply(const dimensions<M> dims, FuncEstimator<T,M,D> f_estimator):
        m_dimensions(dims),
        m_f_estimator(f_estimator),
        m_fmm(
            dims,
            [&](const gs::box<M>& boxImp, const box_val& boxVal, gs::grid<M, grid_val, box_val>& grid){
                /** 
                // Estimate the value at the grid point using taylor
                const int32_t level = static_cast<int32_t>(boxImp.get_level());
                for( const auto& corner : boxImp){
                    auto& gridVal = grid[corner];
                    // Only update if not already updated.
                    if(level != std::get<3>(gridVal)){
                        std::get<0>(gridVal) += m_f_estimator.estimate(boxVal.first, boxVal.second, std::get<2>(gridVal));
                        std::get<3>(gridVal) = static_cast<int32_t>(level);
                    }
                }
    
                const auto cornerVals = analytic_multiply::corner_vals(boxImp, grid);

                // Compute the precise value for the near field by multiplying 
                // and summing over all values.
                for( const auto& corner : boxImp){
                    auto& gridVal = grid[corner];
                    for(uint8_t i=0; i<nBoxCorners; ++i){
                        std::get<0>(gridVal) +=  (
                            m_f_estimator(cornerVals.first[i], std::get<2>(gridVal))*
                            cornerVals.second[i]
                        );
                    }
                }
                */
            },
            [&](const gs::box<M>& boxImp, box_val& boxVal, const gs::grid<M, grid_val, box_val>& grid){
                // Find the corners and values
                const auto cornerVals = analytic_multiply::corner_vals(boxImp, grid);
                // Find the center of hte box
                const auto center = mean(cornerVals.first);
                // Compute the parameters
                // @TODO add parameters from lower levels.
                auto poly = m_f_estimator.compute_coefs(cornerVals.first, center, cornerVals.second);
                boxVal.first = poly;
                boxVal.second = center;
            }
        ) {}

    /**
     * \brief Initialise the grid with the input values
     */
    void initialise(const std::vector<T>& init_vec){
        if(m_fmm.grid_size() != init_vec.size()){
            throw std::range_error("Incorrect size");
        }
        // Initialise the grid
        for (size_t i=0; i<m_fmm.grid_size(); ++i){
            m_fmm[i] = std::tuple<double, double, gs::vector<double, M>, int32_t>{
                0.0, // Output value
                init_vec[i], // Input Value
                gs::vector<double, M>(
                    m_dimensions.ind2sub(i, m_dimensions.max_level())
                ), // Position
                -1,  // Level computed at
            };
        }
    }

    /**
     * \brief Compute the solution
     */
    void compute(const uint32_t nIters=1) {
        m_fmm.compute(nIters);
    }

    /**
     * \brief Return the output
     */
    std::vector<T> output() const{
        const size_t size = m_fmm.grid_size();
        std::vector<T> out;
        out.reserve(size);
        for (size_t i=0; i<size; ++i){
            out.push_back(std::get<0>(m_fmm[i]));
        }
        return out;
    }
};
}

#endif