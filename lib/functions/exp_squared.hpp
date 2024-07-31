
#ifndef _GS_EXP_SQUARED_
#define _GS_EXP_SQUARED_

#include <cmath>

#include "concepts.hpp"

namespace gs {
/**
 * The Exp Squared covariance function.
 */
template<int N, typename T, class Vector, class Matrix>
requires std::is_floating_point<T>::value && is_vector<Vector> && is_matrix<Matrix>
class exp_squared {
    T m_sigma_squared;

    /**
     * Coefficients of the first derivative with respect to x_i
     */
    Vector dy_coef(const Vector& x, const Vector& y) const {
        return (x - y) / m_sigma_squared;
    }

    /**
     * Coefficients of the second derivative with respect to x_i, x_j
     */
    Matrix ddy_coef(const Vector& x, const Vector& y) const {
        Matrix ret(outer(x - y) / m_sigma_squared);
        for (size_t i=0; i<N; ++i){
            ret(i,i) += 1;
        }
        return ret / m_sigma_squared;
    }
public:
    exp_squared(T sigma): m_sigma_squared(sigma*sigma) {}

    T operator()(const Vector& x, const Vector& y){
        return std::exp( - (x-y).norm2() / (2*m_sigma_squared) );
    }

    template<int K>
    std::pair<Vector, Matrix> approx_coef(const std::array<Vector, K>& xVals, const Vector& center) {
        std::pair<Vector, Matrix> coefs;
        for(size_t i=0; i<K; ++i) {
            coefs.first += xVals[i] - center;
            coefs.second += outer(xVals[i] - center);
        }
        return coefs;
    }
};
}

#endif