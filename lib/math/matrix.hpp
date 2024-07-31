
#ifndef _GS_MATRIX_
#define _GS_MATRIX_

#include <iostream>
#include <cmath>
#include <array>

#include "vector.hpp"

namespace gs {
/**
 * A stack allocated matrix.
 */
template<typename T, size_t M, size_t N>
class matrix: public vector<T, M * N> {
public:
    T& operator()(const size_t i, const size_t j){
        return vector<T, M * N>::operator()(N*i + j);
    }
    const T& operator()(const size_t i, const size_t j) const {
        return vector<T, M * N>::operator()(N*i + j);
    }
};

template<typename T, size_t M, size_t N, size_t K>
matrix<T,M,N> operator*(const matrix<T,M,K>& matA, const matrix<T,K,N>& matB ){
    matrix<T,M,N> ret;
    for(size_t i=0; i<M; ++i){
        for(size_t j=0; j<N; ++j){
            for(size_t k=0; k<K; ++k){
                ret(i,j) += matA(i,k)*matB(k,j);
            }
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
vector<T,M> operator*(const matrix<T,M,N>& mat, const vector<T,N>& vec ){
    vector<T,M> ret;
    for(size_t i=0; i<M; ++i){
        for(size_t j=0; j<N; ++j){
            ret(i) += mat(i,j)*vec(j);
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
matrix<T,M,N> outer(const vector<T,M>& vec){
    matrix<T,M,N> ret;
    for(size_t i=0; i<M; ++i){
        for(size_t j=0; j<N; ++j){
            ret(i,j) += vec(i)*vec(j);
        }
    }
    return ret;
}

template<typename T, size_t M, size_t N>
std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& mat)
{
    os << "[";
    for(size_t i=0; i<N; ++i){
        if(i>0){
            os << " ";
        }
        for(size_t j=0; j<N-1; ++j){
            os << mat(i,j) << ",";
        }
        os << mat(i,N-1) << ((i==N-1)?"]":";") << std::endl;
        
    }
    return os;
}
}

#endif