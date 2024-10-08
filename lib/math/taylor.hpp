// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_MATH_TAYLOR_HPP_
#define LIB_MATH_TAYLOR_HPP_

#include "math/polynomial.hpp"
#include "math/vector.hpp"

namespace gs {
template<
    typename T, size_t M, size_t D,
    template < typename, size_t, size_t > class F
>
requires requires(F<T, M, D> f) {
    {
        f(gs::vector<T, M>(), vector<T, M>())
    } -> std::convertible_to<gs::vector<T, pow<M, D>()>>;
}
/**
 * \brief Taylors expansion of a multivariate function.
 * 
 * The Taylors expansion is a polynomial estimate of an arbitrary 
 * continuous and continuously differentiable function about a point.
 * Converting the function into a polynomial is not only a potentially
 * efficient way to compute it, but also allows one to separate variables
 * which are coupled through a non-linear function (such as in exp_squared).
 * Doing so means that we can convert a multi-body type O(N^2) algorithm, in which
 * every point is compared with every other point, into an O(N) algorithm in which
 * every point is only evaluated once (the FMM for example).
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      M - The number of rows.
 *      D - The degree of polynomial to estimate to.
 *      F - The input function type (must be callable).
 */
class taylor {
    F<T, M, D> m_function;  ///< The function to approximate.

 public:
    explicit taylor(const F<T, M, D> func): m_function(func) {}

    template<size_t K = D>
    /**
     * \brief Produce the Taylor estimate of the input function.
     * 
     * Produce an estimate of the function with varying parameter x, fixed
     * parameter y, about the center cx.
     */
    T estimate(
        const gs::vector<T, M>& x,
        const gs::vector<T, M>& cx,
        const gs::vector<T, M>& y
    ) {
        const auto fAtY = static_cast<F<T, M, K>>(m_function)(cx, y);
        if constexpr ( K == 0 ) {
            return fAtY;
        } else if constexpr ( K == 1 ) {
            return fAtY.dot(x-cx) + estimate<K-1>(x, cx, y);
        } else if constexpr ( K == 2 ) {
            return 0.5*(fAtY*(x-cx)).dot(x-cx) + estimate<K-1>(x, cx, y);
        } else {
            return (
                (1.0/factorial<K>())*fAtY.inner(x-cx) +
                estimate<K-1>(x, cx, y)
            );
        }
    }

    template<size_t K = D>
    /**
     * \brief Produce the Taylor estimate of the input function.
     * 
     * Supposing that the function to be estimated is f(x,y), the Taylor
     * expansion about a center c, for the variable x and constant y, is,
     * 
     *   f(x,y) ~ f(c,y) + Jf(c,y)(x-c) + (1/2)*(x-c)^T Hf(c,y) (x-c) + h.o.t.
     * 
     * Supposing that there are several values x_0, ..., x_N in R^M, and 
     * values a_0, ..., a_N in R, then it follows that,
     * that,
     *  ___                              / ___           \
     *  \    f(x_i,y) a_i = (Na)f(c,y) + | \  (x_i-c) a_i | Jf(c,y) + (1/2)*<Q, Hf(c,y)>_F + h.o.t.
     *  /__ i                            \ /__ i         /
     * 
     * Where,
     *       ___
     *  Q =  \   (x_i-c)(x_i-c)^T 
     *       /__ i
     *  a = the sum over a_i
     * and <.,.>_F is the Frobenius inner product.
     * 
     * The coefficients of the derivatives of f are a multivariate polynomial, which
     * can be computed independently of y and c, and ultimately the function estimate.
     */  
    T estimate(
        const polynomial<T, M, D>& polyCoefs,
        const gs::vector<T, M>& cx,
        const gs::vector<T, M>& y
    ) const {
        const auto& funcRef = static_cast<F<T, M, K>>(m_function);
        const auto& polyCoefsRef = static_cast<polynomial<T, M, K>>(polyCoefs);
        if constexpr ( K == 0 ) {
            return funcRef(cx, y - cx)*polyCoefsRef.coeffs();
        } else {
            return (
                (1.0/factorial<K>())*
                polyCoefsRef.coeffs().dot(funcRef(cx, y - cx)) +
                estimate<K-1>(polyCoefs, cx, y)
            );
        }
    }
};
}  // namespace gs

#endif  // LIB_MATH_TAYLOR_HPP_
