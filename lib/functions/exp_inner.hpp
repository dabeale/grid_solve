// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_FUNCTIONS_EXP_INNER_HPP_
#define LIB_FUNCTIONS_EXP_INNER_HPP_

#include <cmath>

#include "base/dimensions.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "base/concepts.hpp"
#include "math/equi_tensor.hpp"

namespace gs {
template<typename T, size_t M, size_t D = 0>
/**
 * \brief The Exp Inner Product function.
 * 
 * The Exp Inner Product function is defined as,
 *                /  x^T y - 0.5 ||y||^2  \
 *   f(x,y) = exp |  -------------------  |
 *                \        sigma^2        /
 * The implementation computes both the function and
 * its multivariate derivatives on the stack. 
 * 
 * The template parameters,
 *      T - The base type (e.g. double).
 *      M - The number of dimensions in the input vectors.
 *      D - The degree of the derivative.
 *          (evaluation of the object at D returns a D-Tensor).
 */
class exp_inner: public exp_inner<T, M, D-1> {
 protected:
    dimensions<D> m_dimensions;  ///< The dimensions of the tensor.

 public:
    explicit exp_inner(T sigma = 1):
        exp_inner<T, M, D-1>(sigma), m_dimensions(M, 0) {}

    /**
     * \brief Evaluate the function / derivative.
     * 
     * Depending on the value of D, return the evaluation of 
     * the exp_inner function at x and y. When D==0
     * this is simply the function itself, but at higher values
     * it is the Dth derivative.
     */
    equi_tensor<T, D, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        const equi_tensor<T, D, M> tOuter = tensor_outer<T, D, M>(
            exp_inner<T, M, 0>::d_coef(x, y));
        return equi_tensor<T, D, M>(
            tOuter*exp_inner<T, M, 0>::operator()(x, y));
    }
};

template<typename T, size_t M>
/**
 * \brief The specialisation of exp_inner for the second derivative.
 */
class exp_inner<T, M, 2>: public exp_inner<T, M, 1> {
 protected:
    dimensions<2> m_dimensions;

 public:
    explicit exp_inner(T sigma = 1):
        exp_inner<T, M, 1>(sigma), m_dimensions(M, 0) {}

    matrix<T, M, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        return matrix<T, M, M>(
            matrix_outer(exp_inner<T, M, 0>::d_coef(x, y))*
            exp_inner<T, M, 0>::operator()(x, y));
    }
};

template<typename T, size_t M>
/**
 * \brief The specialisation of exp_inner for the first derivative.
 */
class exp_inner<T, M, 1>: public exp_inner<T, M, 0> {
 protected:
    dimensions<1> m_dimensions;

 public:
    explicit exp_inner(T sigma = 1):
        exp_inner<T, M, 0>(sigma), m_dimensions(M, 0) {}

    gs::vector<T, M> operator()(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& y)
    const {
        return (
            exp_inner<T, M, 0>::d_coef(x, y)*
            exp_inner<T, M, 0>::operator()(x, y));
    }
};


template<typename T, size_t M>
/**
 * \brief The specialisation of exp_inner for the zeroth derivative.
 */
class exp_inner<T, M, 0> {
 protected:
    T m_sigma_squared;  ///< The variance (std-dev squared)

 public:
    explicit exp_inner(T sigma = 1):
        m_sigma_squared(sigma*sigma) {}
    gs::vector<T, M> d_coef(
        const gs::vector<T, M>&, const gs::vector<T, M>& y)
    const {
        return y/m_sigma_squared;
    }
    T operator()(const gs::vector<T, M>& x, const gs::vector<T, M>& y) const{
        return std::exp( (2*x.dot(y) - y.norm2()) / (2*m_sigma_squared) );
    }
};
}  // namespace gs

#endif  // LIB_FUNCTIONS_EXP_INNER_HPP_
