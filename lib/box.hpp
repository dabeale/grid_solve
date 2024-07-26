
#ifndef _GS_BOX_
#define _GS_BOX_

#include <iostream>

#include "index.hpp"
#include "tools.hpp"

namespace gs {
/**
 * \brief A box in a grid.
 */
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value
class box {
    std::array<index<N, T>, pow<2,N>()> m_corners; ///< The corners
    T m_level; ///< The box level

public:
    static constexpr T m_nCorners = pow<2,N>(); ///< The number of corners
    static constexpr T m_nSubPoints = pow<3,N>(); ///< The number of subpoints

    box(const T level, const T offset=0): m_level(level) {
        for (T i = 0; i<m_nCorners; ++i){
            std::array<T, N> indices;
            for (T j = 0; j < N; ++j){
                indices[j] = ((((1 << j) & i) >> j) + ((offset & (1 << j)) >> j));
            }
            m_corners[i] = index<N,T>(indices, level);
        }
    }
    box(const std::array<index<N, T>, m_nCorners> corners, const T level):
        m_corners(corners), m_level(level)
    {}

    /**
     * Find the neighbour box in the specified dimension
     */
    box<N, T> neighbour(const T dim, const typename index<N,T>::PosNeg direction) const{
        std::array<index<N, T>, m_nCorners> newCorners;
        for (T i = 0; i < m_nCorners; ++i){
            newCorners[i] = m_corners[i].neighbour(dim, direction);
        }
        return box<N,T>(newCorners, m_level);
    }

    /**
     * Find all of the subboxes after binary subdivision.
     */
    box<N,T> subbox(const T ind) const{
        return box<N,T>(m_level - 1, ind);
    }

    /**
     * Find all of the subpoints after binary subdivision.
     * This does not include the corners of the box.
     */
    std::array<index<N, T>, m_nSubPoints - m_nCorners> subpoints(){
         std::array<index<N, T>, m_nSubPoints - m_nCorners> innerPoints;
         std::array<T, N> ternary;
         T k = 0;
         for (T i = 0; i < m_nSubPoints; ++i){
            T acc = 1;
            bool allEdge = true;
            for (T j = 0; j<N; ++j){
                ternary[j] = (i / acc)  % 3;
                allEdge &= (ternary[j] != 1);
                acc *= 3;
            }
            if (!allEdge && k < m_nSubPoints - m_nCorners){
                innerPoints[k++] = m_corners[0] + index<N,T>(ternary, m_level-1);
            }
         }
         return innerPoints;
    }

    const auto& operator[](const T i) const{return m_corners[i];}
    auto begin() -> decltype(m_corners.begin()){return m_corners.begin();}
    auto end() -> decltype(m_corners.end()){return m_corners.end();}
    auto begin() const -> decltype(m_corners.begin()){return m_corners.begin();}
    auto end() const -> decltype(m_corners.end()){return m_corners.end();}
};

template<int N, typename T=uint32_t>
std::ostream& operator<<(std::ostream& os, const box<N, T>& boxVar)
{
    for (auto corner : boxVar){
        os << corner << " ";
    }
    return os;
}

}

#endif