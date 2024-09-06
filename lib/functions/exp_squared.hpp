// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_FUNCTIONS_EXP_SQUARED_HPP_
#define LIB_FUNCTIONS_EXP_SQUARED_HPP_

#include <cmath>

#include "base/dimensions.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "base/concepts.hpp"
#include "math/equi_tensor.hpp"

namespace gs {
template<typename T, size_t M, size_t D = 0>
/**
 * \brief The Exp-Squared covariance function.
 * 
 * The Exp-Squared covariance function is defined as,
 *                /  -|| x - y ||^2   \
 *   f(x,y) = exp |  ---------------- |
 *                \     2*sigma^      /
 * The implementation computes both the function and
 * its multivariate derivatives on the stack. 
 * 
 * The template parameters,
 *      T - The base type (e.g. double).
 *      M - The number of dimensions in the input vectors.
 *      D - The degree of the derivative.
 *          (evaluation of the object at D returns a D-Tensor).
 */
class exp_squared: public exp_squared<T, M, D-1> {
 protected:
    dimensions<D> m_dimensions;  ///< The dimensions of the tensor.

 public:
    explicit exp_squared(T sigma = 1):
        exp_squared<T, M, D-1>(sigma), m_dimensions(M, 0) {}

    /**
     * \brief Evaluate the function / derivative.
     * 
     * Depending on the value of D, return the evaluation of 
     * the exp_squared covariance function at x and y. When D==0
     * this is simply the function itself, but at higher values
     * it is the Dth derivative.
     */
    equi_tensor<T, D, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        equi_tensor<T, D, M> ret;
        const auto pTens = exp_squared<T, M, D-1>::operator()(x, y);
        const auto ppTens = exp_squared<T, M, D-2>::operator()(x, y);
        const auto dCoef = exp_squared<T, M, 0>::d_coef(x, y);
        const auto& pDims = exp_squared<T, M, D-1>::m_dimensions;
        const auto& ppDims = exp_squared<T, M, D-2>::m_dimensions;
        for ( size_t i = 0; i < pow<M, D>(); ++i ) {
            auto index = m_dimensions.ind2sub(i);
            ret[i] = pTens[
                pDims.sub2ind(remove_i<uint32_t, D>(index, 0u))
            ]*dCoef[index[0]];
            for ( size_t k = 1; k < D; ++k ) {
                if (index[0] == index[k]) {
                    ret[i] -= ppTens[
                        ppDims.sub2ind(remove_i<uint32_t, D>(index, 0u, k))
                    ] / exp_squared<T, M, 0>::m_sigma_squared;
                }
            }
        }
        return ret;
    }
};

template<typename T, size_t M>
/**
 * \brief The specialisation of exp_squared for the second derivative.
 */
class exp_squared<T, M, 2>: public exp_squared<T, M, 1> {
 protected:
    dimensions<2> m_dimensions;

 public:
    explicit exp_squared(T sigma = 1):
        exp_squared<T, M, 1>(sigma), m_dimensions(M, 0) {}

    matrix<T, M, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        T fEval = exp_squared<T, M, 0>::operator()(x, y);
        matrix<T, M, M> ret(
            matrix_outer(
                exp_squared<T, M, 1>::operator()(x, y),
                exp_squared<T, M, 0>::d_coef(x, y)));
        for ( size_t m = 0; m < M; ++m ) {
            ret(m, m) -= fEval / exp_squared<T, M, 0>::m_sigma_squared;
        }
        return ret;
    }
};

template<typename T, size_t M>
/**
 * \brief The specialisation of exp_squared for the first derivative.
 */
class exp_squared<T, M, 1>: public exp_squared<T, M, 0> {
 protected:
    dimensions<1> m_dimensions;

 public:
    explicit exp_squared(T sigma = 1):
        exp_squared<T, M, 0>(sigma), m_dimensions(M, 0) {}

    gs::vector<T, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        return (
            exp_squared<T, M, 0>::d_coef(x, y)*
            exp_squared<T, M, 0>::operator()(x, y));
    }
};

template<typename T, size_t M>
/**
 * \brief The specialisation of exp_squared for the zeroth derivative.
 */
class exp_squared<T, M, 0> {
 protected:
    T m_sigma_squared;  ///< Sigma squared parameter (variance).

 public:
    explicit exp_squared(T sigma = 1): m_sigma_squared(sigma*sigma) {}
    gs::vector<T, M> d_coef(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        return (x-y)/(-m_sigma_squared);
    }
    T operator()(const gs::vector<T, M>& x, const gs::vector<T, M>& y) const{
        return std::exp( - (x-y).norm2() / (2*m_sigma_squared) );
    }
};
}  // namespace gs

#endif  // LIB_FUNCTIONS_EXP_SQUARED_HPP_
