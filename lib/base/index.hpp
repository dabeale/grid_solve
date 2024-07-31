
#ifndef _GS_INDEX_
#define _GS_INDEX_

#include <iostream>
#include <inttypes.h>
#include <array>

#include "base/concepts.hpp"
#include "base/dimensions.hpp"

namespace gs {
/**
 * \brief An index within a grid.
 * 
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
    index(const std::array<T, N> indices, const T level): m_level(level), m_indices(indices){}

    /**
     * Return the level of the index.
     */
    T get_level() const {
        return m_level;
    }

    /**
     * Change the level of the index.
     */
    void set_level(const T level){
        T mult = (1 << (level - m_level));
        for (auto& pt : m_indices){
            pt = pt * mult;
        }
        m_level = level;
    }

    /**
     * Create a new index of specified level and return it.
     */
    index<N, T> at_level(const T level) const {
        index<N, T> newIndex(*this);
        newIndex.set_level(level);
        return newIndex;
    }

    /**
     * Add to another index.
     */
    index<N,T> operator+(index<N,T> other) const{
        index<N,T> newRet(*this);
        newRet += other;
        return newRet;
    }
    /**
     * Add to another index.
     */
    const index<N,T>& operator+=(index<N,T> other){
        for (T k=0; k<N; ++k){
            m_indices[k] += other.m_indices[k];
        }
        return *this;
    }

    /**
     * Return the index at the current level.
     */
    const T& operator[](const T i) const {return m_indices[i];}
    T& operator[](const T i) {return m_indices[i];}

    /**
     * Return the index at the lowest level.
     */
    T operator()(const T i) const {return m_indices[i]*(1 << m_level);}
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
requires random_access_container<T> && std::is_integral<S>::value
bool operator==(index<N, S> ind, T arr){
    for (S i = 0; i < N; ++i){
        if (arr[i] != ind[i]) return false;
    }
    return true;
}
}

#endif