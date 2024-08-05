
#ifndef _GS_TAYLOR_
#define _GS_TAYLOR_

#include "math/polynomial.hpp"
#include "math/vector.hpp"

namespace gs {
/**
 * \brief Taylors expansion of a multivariate function.
 * 
 * The Taylors expansion is a polynomial estimate of an arbitrary 
 * continuous and continuously differentiable function about a point.
 * Converting the function into a polynomial is not only a potentially
 * efficient way to compute it, but also allows one to separate variables
 * which are coupled through a non-linear function (such as in exp_squared).
 * Doing so means that we can compute a multi-body type O(N^2) algorithm, in which
 * every point is compared with every other point, into an O(N) algorith in which
 * every point is only once.
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      M - The number of rows.
 *      D - The degree of polynomial to estimate to.
 *      F - The input function type (must be callable).
 */
template< typename T, size_t M, size_t D, template < typename, size_t, size_t > class F >
requires requires(F<T, M, D> f) {
    { f(vector<T,M>(), vector<T,M>()) } -> std::convertible_to<vector<T, pow<M,D>()>>;
}
class taylor {
    F<T, M, D> m_function; ///< The function to approximate.

public:
    taylor(const F<T, M, D> func): m_function(func){}

    /**
     * \brief Produce the Taylor estimate of the input function.
     * 
     * Produce an estimate of the function with varying parameter x, fixed
     * parameter y, about the center cx.
     */
    template<size_t K=D>
    T estimate(const vector<T, M>& x, const vector<T, M>& cx, const vector<T, M>& y){
        if constexpr ( K == 0 ) {
            return static_cast<F<T, M, K>>(m_function)(cx, y);
        }
        else if constexpr ( K == 1 ){
            return static_cast<F<T, M, K>>(m_function)(cx, y).dot(x-cx) + estimate<K-1>(x,cx,y);
        }
        else if constexpr ( K == 2 ){
            return 0.5*(static_cast<F<T, M, K>>(m_function)(cx, y)*(x-cx)).dot(x-cx) + estimate<K-1>(x,cx,y);
        }
        else {
            return (1.0/factorial<K>())*static_cast<F<T, M, K>>(m_function)(cx, y).inner(x-cx) + estimate<K-1>(x,cx,y);
        }
    }
};
}

#endif