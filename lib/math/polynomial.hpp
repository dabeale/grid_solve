
#ifndef _GS_POLYNOMIAL_
#define _GS_POLYNOMIAL_

#include <array>

#include "equi_tensor.hpp"
#include "matrix.hpp"
#include "vector.hpp"

namespace gs {
/**
 * A stack allocated polynomial.
 */
template<typename T, size_t D, size_t N>
class polynomial : public polynomial<T, D-1, N>{
protected:
    equi_tensor<T, D, N> m_coeff;
public:
    /**
     * Fill the parameters using a collection of vectors.
     * The parameters a computed as the sum of the inner 
     * products of the vectors.
     */
    template<size_t K>
    void fill(const std::array<vector<T, N>, K>& vals){
        for(size_t k=0; k<K; ++k){
            m_coeff += tensor_outer<T, D, K>(vals[k]);
        }
        polynomial<T, D-1, N>::fill(vals);
    }
    /**
     * Evaluate the polynomial at the specified vector.
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