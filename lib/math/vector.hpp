
#ifndef _GS_VECTOR_
#define _GS_VECTOR_

#include <cmath>
#include <array>
#include <iostream>

namespace gs {
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
template<typename T, size_t M>
class vector {
protected:
    std::array<T, M> m_array;
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
    T& operator()(const size_t i){
        return m_array[i];
    }
    const T& operator()(const size_t i) const {
        return m_array[i];
    }
    T& operator[](const size_t i){
        return m_array[i];
    }
    const T& operator[](const size_t i) const {
        return m_array[i];
    }
    const vector<T,M>& operator+=(const vector<T,M>& other){
        for(size_t i=0; i<M; ++i) m_array[i] += other.m_array[i];
        return *this;
    }
    const vector<T,M>& operator-=(const vector<T,M>& other){
        for(size_t i=0; i<M; ++i) m_array[i] -= other.m_array[i];
        return *this;
    }
    const vector<T,M>& operator+=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] += c;
        return *this;
    }
    const vector<T,M>& operator-=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] -= c;
        return *this;
    }
    const vector<T,M>& operator*=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] *= c;
        return *this;
    }
    const vector<T,M>& operator/=(const T& c){
        for(size_t i=0; i<M; ++i) m_array[i] /= c;
        return *this;
    }
    vector<T,M> operator+(const vector<T,M>& other) const{
        return (vector<T,M>(*this) += other);
    }
    vector<T,M> operator-(const vector<T,M>& other) const{
        return (vector<T,M>(*this) -= other);
    }
    vector<T,M> operator/(const T& val) const{
        return (vector<T,M>(*this) /= val);
    }
    vector<T,M> operator*(const T& val) const{
        return (vector<T,M>(*this) *= val);
    }
    T dot(const vector<T,M>& other) const{
        T c = 0;
        for(size_t i=0; i<M; ++i) c += m_array[i]*other.m_array[i];
        return c;
    }
    T norm2() const {
        T c = 0;
        for(size_t i=0; i<M; ++i) c += m_array[i]*m_array[i];
        return c;
    }
};

template<typename T, size_t M>
std::ostream& operator<<(std::ostream& os, const vector<T, M>& vec)
{
    os << "[";
    for(size_t i=0; i<M-1; ++i){
        os << vec(i) << ",";
    }
    os << vec(M-1) << "]";
    return os;
}
}

#endif