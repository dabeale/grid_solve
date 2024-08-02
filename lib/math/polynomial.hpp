
#ifndef _GS_POLYNOMIAL_
#define _GS_POLYNOMIAL_

#include <array>

#include "math/equi_tensor.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

namespace gs {
/**
 * \brief A stack allocated polynomial.
 * 
 * The multivariate polynomial contains every coefficient 
 * for each monomial, upto the specified degree. Since the
 * polynomial is stack allocated, the degree must be known
 * at compile time. Recursive inheritance is used to store
 * each of the coefficients, which are tensors of dimension 
 * equal to the respective degree.
 * 
 * For example, a second order polynomial is,
 *  f(x) = a + x^t b + x^t C x
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      D - The degree.
 *      N - The number of input dimensions.
 */
template<typename T, size_t D, size_t N>
class polynomial : public polynomial<T, D-1, N>{
protected:
    equi_tensor<T, D, N> m_coeff;
public:
    /**
     * \brief Fill the parameters using a collection of vectors.
     * 
     * The parameters a computed as the sum of the inner 
     * products of the vectors.
     */
    template<size_t K>
    void fill(const std::array<vector<T, N>, K>& vals){
        for(size_t k=0; k<K; ++k){
            m_coeff += tensor_outer<T, D, N>(vals[k]);
        }
        polynomial<T, D-1, N>::fill(vals);
    }
    /**
     * \brief Evaluate the polynomial at the specified vector.
     */
    T evaluate(const vector<T, N>& vin){
        return m_coeff.inner(vin) + polynomial<T, D-1, N>::evaluate(vin);
    }
};

template<typename T, size_t N>
class polynomial<T, 2, N>: polynomial<T, 1, N> {
protected:
    matrix<T, N, N> m_coeff;
public:
    template<size_t K>
    void fill(const std::array<vector<T, N>, K>& vals){
        for(size_t k=0; k<K; ++k){
            m_coeff += matrix_outer<T, N>(vals[k]);
        }
        polynomial<T, 1, N>::fill(vals);
    }
    T evaluate(const vector<T, N>& vin){
        return (m_coeff*vin).dot(vin) + polynomial<T, 1, N>::evaluate(vin);
    }
};

template<typename T, size_t N>
class polynomial<T, 1, N>: polynomial<T, 0, N> {
protected:
    vector<T, N> m_coeff;
public:
    template<size_t K>
    void fill(const std::array<vector<T, N>, K>& vals){
        for(size_t k=0; k<K; ++k){
            m_coeff += vals[k];
        }
        polynomial<T, 0, N>::fill(vals);
    }
    T evaluate(const vector<T, N>& vin){
        return m_coeff.dot(vin) + polynomial<T, 0, N>::evaluate(vin);
    }
};

template<typename T, size_t N>
class polynomial<T, 0, N> {
protected:
    T m_coeff;
public:
    template<size_t K>
    void fill(const std::array<vector<T, N>, K>&){
        m_coeff = 1.0;
    }
    T evaluate(const vector<T, N>&){
        return m_coeff;
    }
};
}

#endif