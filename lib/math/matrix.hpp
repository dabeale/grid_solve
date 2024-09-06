// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_MATH_MATRIX_HPP_
#define LIB_MATH_MATRIX_HPP_

#include <iostream>
#include <cmath>
#include <array>

#include "math/vector.hpp"

namespace gs {

template<typename T, size_t M, size_t N>
/**
 * \brief A stack allocated matrix.
 * 
 * The stack allocated matrix is a matrix which is allocated on the
 * stack. It inherits from a vector, and so all of the basic operations,
 * such as +,-,* etc, inherit from that class. Specific operations such
 * as matrix multiply are also implemented in this class.
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      M - The number of rows.
 *      N - The number of columns.
 */
class matrix: public gs::vector<T, M * N> {
 public:
    matrix(): gs::vector<T, M * N>() {}
    explicit matrix(const gs::vector<T, M * N>& vec):
        gs::vector<T, M * N>(vec) {}

    /**
     * \brief Access the (i,j)th element of the matrix.
     */
    T& operator()(const size_t i, const size_t j) {
        return gs::vector<T, M * N>::operator()(N*i + j);
    }
    /**
     * \brief Access the (i,j)th element of the matrix.
     */
    const T& operator()(const size_t i, const size_t j) const {
        return gs::vector<T, M * N>::operator()(N*i + j);
    }
};

template<typename T, size_t M, size_t N, size_t K>
/**
 * \brief Multiply two matrices.
 */
matrix<T, M, N> operator*(
    const matrix<T, M, K>& matA,
    const matrix<T, K, N>& matB
) {
    matrix<T, M, N> ret;
    for ( size_t i = 0; i < M; ++i ) {
        for ( size_t j = 0; j < N; ++j ) {
            for ( size_t k = 0; k < K; ++k ) {
                ret(i, j) += matA(i, k)*matB(k, j);
            }
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
/**
 * \brief Multiply a matrix and a vector.
 */
gs::vector<T, M> operator*(
    const matrix<T, M, N>& mat,
    const gs::vector<T, N>& vec
) {
    gs::vector<T, M> ret;
    for ( size_t i = 0; i < M; ++i ) {
        for ( size_t j = 0; j < N; ++j ) {
            ret(i) += mat(i, j)*vec(j);
        }
    }
    return ret;
}

template<typename T, size_t M>
/**
 * \brief Compute the vector outer product for a 
 * single input vector.
 */
matrix<T, M, M> matrix_outer(const gs::vector<T, M>& vec) {
    matrix<T, M, M> ret;
    for ( size_t i = 0; i < M; ++i ) {
        for ( size_t j = 0; j < M; ++j ) {
            ret(i, j) += vec(i)*vec(j);
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
/**
 * \brief Compute the vector outer product for a 
 * pair if input vectors.
 */
matrix<T, M, N> matrix_outer(
    const gs::vector<T, M>& vecA,
    const gs::vector<T, N>& vecB
) {
    matrix<T, M, N> ret;
    for ( size_t i = 0; i < M; ++i ) {
        for ( size_t j = 0; j < N; ++j ) {
            ret(i, j) += vecA(i)*vecB(j);
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
/**
 * \brief Append a matrix to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& mat) {
    os << "[";
    for ( size_t i = 0; i < N; ++i ) {
        if ( i > 0 ) {
            os << " ";
        }
        for ( size_t j = 0; j < N-1; ++j ) {
            os << mat(i, j) << ",";
        }
        os << mat(i, N-1) << ((i == N-1) ? "]" : ";") << std::endl;
    }
    return os;
}

template<typename T>
/**
 * \brief Matrix concept.
 */
concept is_matrix = requires(T m, T n) {
    m(int(), int());
    { m+n } -> std::same_as<T>;
    { m*n } -> std::same_as<T>;
    m+double();
    m*double();
    m/double();
    m-double();
};  // NOLINT(readability/braces)
}  // namespace gs

#endif  // LIB_MATH_MATRIX_HPP_
