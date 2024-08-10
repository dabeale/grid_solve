
#ifndef _GS_VECTOR_
#define _GS_VECTOR_

#include <cmath>
#include <array>
#include <iostream>

namespace gs {
template<typename T, size_t M>
/**
 * \brief A stack allocated vector.
 * 
 * The stack allocated vector is simply a stl array, with standard 
 * vector operations defined on it, such as +, -, dot, and norm2.
 * 
 * The vector provides the base class for all higher dimensional
 * tensors. Elements can be access using the call operator, or the 
 * subscript operator.
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      M - The number of dimensions.
 */
class vector {
protected:
    std::array<T, M> m_array; ///< The storage array.
public:
    vector() {
        m_array.fill(0);
    }
    vector(std::initializer_list<T> inList) {
        size_t j=0;
        for(const auto i : inList){
            m_array[j] = i;
            j++;
        }
    }

    T& operator()(const size_t i){ return m_array[i]; } ///< Access the ith element of the vector
    const T& operator()(const size_t i) const {return m_array[i];}  ///< Access the ith element of the vector
    T& operator[](const size_t i){return m_array[i];} ///< Access the ith element of the vector
    const T& operator[](const size_t i) const {return m_array[i];} ///< Access the ith element of the vector

    /**
     * \brief Add to another vector in-place.
     */
    const vector<T,M>& operator+=(const vector<T,M>& other){
        for(size_t i=0; i<M; ++i) m_array[i] += other.m_array[i];
        return *this;
    }
    /**
     * \brief Negate from another vector in-place.
     */
    const vector<T,M>& operator-=(const vector<T,M>& other){
        for(size_t i=0; i<M; ++i) m_array[i] -= other.m_array[i];
        return *this;
    }
    /**
     * \brief Add to a constant in place.
     */
    const vector<T,M>& operator+=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] += c;
        return *this;
    }
    /**
     * \brief Negate from a constant in place.
     */
    const vector<T,M>& operator-=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] -= c;
        return *this;
    }
    /**
     * \brief Multiply by constant in place.
     */
    const vector<T,M>& operator*=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] *= c;
        return *this;
    }
    /**
     * \brief Divide by constant in place.
     */
    const vector<T,M>& operator/=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] /= c;
        return *this;
    }
    /**
     * \brief Add to another vector and return the result.
     */
    vector<T,M> operator+(const vector<T,M>& other) const{
        return (vector<T,M>(*this) += other);
    }
    /**
     * \brief Negate from another vector and return the result.
     */
    vector<T,M> operator-(const vector<T,M>& other) const{
        return (vector<T,M>(*this) -= other);
    }
    /**
     * \brief Divide by a constant and return the result.
     */
    vector<T,M> operator/(const T& val) const{
        return (vector<T,M>(*this) /= val);
    }
    /**
     * \brief Multiply by a constant and return the result.
     */
    vector<T,M> operator*(const T& val) const{
        return (vector<T,M>(*this) *= val);
    }
    /**
     * \brief Return the dot product with another vector.
     */
    T dot(const vector<T,M>& other) const{
        T c = 0;
        for(size_t i=0; i<M; ++i) c += m_array[i]*other.m_array[i];
        return c;
    }
    /**
     * \brief Return the square of the Euclidean norm.
     */
    T norm2() const {
        T c = 0;
        for(size_t i=0; i<M; ++i) c += m_array[i]*m_array[i];
        return c;
    }
    /**
     * \brief Return the Euclidean norm.
     */
    T norm() const {
        return std::sqrt(norm2());
    }
};

template<typename T, size_t M>
/**
 * \brief Append the vector to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const vector<T, M>& vec)
{
    os << "[";
    for(size_t i=0; i<M-1; ++i){
        os << vec(i) << ",";
    }
    os << vec(M-1) << "]";
    return os;
}

template<typename T>
/**
 * \brief Vector concept.
 */
concept is_vector = random_access<T> && requires(T m, T n) {
    m(int());
    { m+n } -> std::same_as<T>;
    m+double();
    m*double();
    m/double();
    m-double();
};
}

#endif