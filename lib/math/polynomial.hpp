// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_MATH_POLYNOMIAL_HPP_
#define LIB_MATH_POLYNOMIAL_HPP_

#include <array>

#include "math/equi_tensor.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

namespace gs {
template<typename T, size_t N, size_t D>
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
 *      N - The number of input dimensions.
 *      D - The degree.
 */
class polynomial : public polynomial<T, N, D-1>{
 protected:
    equi_tensor<T, D, N> m_coeff;

 public:
    polynomial(): polynomial<T, N, D-1>(), m_coeff() {}
    template<size_t K>
    polynomial(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) { fill(vectorVals, tVals); }
    template<size_t K>
    /**
     * \brief Fill the parameters using a collection of vectors.
     * 
     * The parameters a computed as the sum of the inner 
     * products of the weighted vectors. Computing the parameters
     * in this way, is a principle component of the Taylor's expansion.
     */
    void fill(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) {
        for ( size_t k = 0; k < K; ++k ) {
            m_coeff += tensor_outer<T, D, N>(vectorVals[k])*tVals[k];
        }
        polynomial<T, N, D-1>::fill(vectorVals, tVals);
    }
    /**
     * \brief Evaluate the polynomial at the specified vector.
     */
    T evaluate(const gs::vector<T, N>& vin) {
        return m_coeff.inner(vin) + polynomial<T, N, D-1>::evaluate(vin);
    }
    /**
     * \brief Return the coefficients of the polynomial at the
     * current degree.
     */
    const equi_tensor<T, D, N>& coeffs() const {
        return m_coeff;
    }
    /**
     * \brief Add another polynomial.
     */
    const polynomial<T, N, D>& operator+=(const polynomial<T, N, D>& other) {
        m_coeff += other.m_coeff;
        polynomial<T, N, D-1>::operator+=(other);
        return *this;
    }
};

template<typename T, size_t N>
/**
 * \brief Specialisation for a multivariate polynomial of order 2.
 */
class polynomial<T, N, 2>: public polynomial<T, N, 1> {
 protected:
    matrix<T, N, N> m_coeff;

 public:
    polynomial(): polynomial<T, N, 1>(), m_coeff() {}
    template<size_t K>
    polynomial(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) { fill(vectorVals, tVals); }
    template<size_t K>
    void fill(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) {
        for ( size_t k = 0; k < K; ++k ) {
            m_coeff += matrix_outer<T, N>(vectorVals[k])*tVals[k];
        }
        polynomial<T, N, 1>::fill(vectorVals, tVals);
    }
    T evaluate(const gs::vector<T, N>& vin) {
        return (m_coeff*vin).dot(vin) + polynomial<T, N, 1>::evaluate(vin);
    }
    const matrix<T, N, N>& coeffs() const {
        return m_coeff;
    }
    const polynomial<T, N, 2>& operator+=(const polynomial<T, N, 2>& other) {
        m_coeff += other.m_coeff;
        polynomial<T, N, 1>::operator+=(other);
        return *this;
    }
};

template<typename T, size_t N>
/**
 * \brief Specialisation for a multivariate polynomial of order 1.
 */
class polynomial<T, N, 1>: public polynomial<T, N, 0> {
 protected:
    gs::vector<T, N> m_coeff;

 public:
    polynomial(): polynomial<T, N, 0>(), m_coeff() {}
    template<size_t K>
    polynomial(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) { fill(vectorVals, tVals); }
    template<size_t K>
    void fill(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) {
        for ( size_t k = 0; k < K; ++k ) {
            m_coeff += vectorVals[k]*tVals[k];
        }
        polynomial<T, N, 0>::fill(vectorVals, tVals);
    }
    T evaluate(const gs::vector<T, N>& vin) {
        return m_coeff.dot(vin) + polynomial<T, N, 0>::evaluate(vin);
    }
    const gs::vector<T, N>& coeffs() const {
        return m_coeff;
    }
    const polynomial<T, N, 1>& operator+=(const polynomial<T, N, 1>& other) {
        m_coeff += other.m_coeff;
        polynomial<T, N, 0>::operator+=(other);
        return *this;
    }
};

template<typename T, size_t N>
/**
 * \brief Specialisation for a multivariate polynomial of order 0.
 */
class polynomial<T, N, 0> {
 protected:
    T m_coeff;

 public:
    polynomial(): m_coeff(0.0) {}
    template<size_t K>
    polynomial(
        const std::array<gs::vector<T, N>, K>& vectorVals,
        const std::array<T, K>& tVals
    ) { fill(vectorVals, tVals); }
    template<size_t K>
    void fill(
        const std::array<gs::vector<T, N>, K>&,
        const std::array<T, K>& tVals
    ) {
        m_coeff = 0.0;
        for ( size_t k = 0; k < K; ++k ) {
            m_coeff += tVals[k];
        }
    }
    T evaluate(const gs::vector<T, N>&) {
        return m_coeff;
    }
    const T& coeffs() const {
        return m_coeff;
    }
    const polynomial<T, N, 0>& operator+=(const polynomial<T, N, 0>& other) {
        m_coeff += other.m_coeff;
        return *this;
    }
};
}  // namespace gs

#endif  // LIB_MATH_POLYNOMIAL_HPP_
