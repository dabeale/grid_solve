
#ifndef _GS_INDEX_
#define _GS_INDEX_

#include <iostream>
#include <inttypes.h>
#include <array>

#include "concepts.h"

namespace gs {
/**
 * \brief An index within a grid
 */
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value
class index {
    std::array<T, N> m_indices; ///< The array of indices
    T m_level; ///< The level within the grid

public:
    index () {}
    index(const T level): m_level(level){
        m_indices.fill(0);
    }
    index(const std::array<T, N> indices, const T level):
        m_level(level){
        for (T i = 0; i < N; ++i){
            m_indices[i] = indices[i]*(1 << m_level);
        }
    }

    /**
     * An enum specifying which side the neighbour
     * should be on.
     */
    enum PosNeg{
        POSITIVE=1,
        NEGATIVE=-1
    };
    /**
     * Get the neighbour of the index in the
     * specified dimension.
     */
    index<N,T> neighbour(const T dim, const PosNeg posNeg) const {
        index<N,T> newIndices(*this);
        newIndices.m_indices[dim] += static_cast<int>(posNeg)* (1 << m_level);
        return newIndices;
    }

    /**
     * Get a one dimensional index representation.
     */
    T sub2ind(const std::array<T, N>& dimensions) const {
        T retInd = m_indices[N-1];
        T coef = dimensions[N-1];
        for (T i=2; i<=N; ++i){
            retInd += coef*m_indices[N-i];
            coef *= dimensions[N-i];
        }
        return retInd;
    }

    index<N,T> operator+(const index<N,T>& other){
        index<N,T> newRet;
        for (T k=0; k<N; ++k){
            newRet.m_indices[k] = m_indices[k] + other.m_indices[k];
        }
        return newRet;
    }
    const T& operator[](const T i) const {return m_indices[i];}
    auto begin() -> decltype(m_indices.begin()){return m_indices.begin();}
    auto end() -> decltype(m_indices.end()){return m_indices.end();}
    auto begin() const -> decltype(m_indices.begin()){return m_indices.begin();}
    auto end() const -> decltype(m_indices.end()){return m_indices.end();}
};

template<int N, typename T=uint32_t>
std::ostream& operator<<(std::ostream& os, const index<N, T>& boxVar)
{
    os << "(";
    T ind = 0;
    for (const auto& corner : boxVar){
        os << corner;
        if ( ind++ < N - 1 ) {
            os << ",";
        }
    }
    os << ")";
    return os;
}
template<int N, typename T, typename S>
requires RandomAccessContainer<T> && std::is_integral<S>::value
bool operator==(index<N, S> ind, T arr){
    for (S i = 0; i < N; ++i){
        if (arr[i] != ind[i]) return false;
    }
    return true;
}

}

#endif