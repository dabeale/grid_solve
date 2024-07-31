
#ifndef _GS_BOX_
#define _GS_BOX_

#include <iostream>
#include <algorithm>

#include "base/dimensions.hpp"
#include "base/index.hpp"
#include "base/tools.hpp"

namespace gs {
/**
 * \brief A box in a grid.
 */
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value
class box {
    std::array<index<N, T>, pow<2,N>()> m_corners; ///< The corners
    T m_level; ///< The box level
    dimensions<N,T> m_dimensions; ///< The dimensions of the box

public:
    static constexpr T m_nCorners = pow<2,N>();   ///< The number of corners
    static constexpr T m_nSubPoints = pow<3,N>(); ///< The number of subpoints

    box(const dimensions<N,T> dimensions, const T level, const T offset=0): m_level(level), m_dimensions(dimensions) {
        const index<N,T> offsetIndex(dimensions.ind2sub(offset, level), level);
        for (T i = 0; i < m_nCorners; ++i){
            m_corners[i] = index<N,T>(dimensions.ind2sub(i), level) + offsetIndex;
        }
    }
    box(const dimensions<N,T> dimensions, const std::array<index<N, T>, m_nCorners> corners, const T level):
        m_corners(corners), m_level(level), m_dimensions(dimensions)
    {}

    /**
     * Get the current level.
     */
    T get_level() const {
        return m_level;
    }

    enum PosNeg {
        POSITIVE=1,
        NEGATIVE=-1
    };
    /**
     * Change the box to its neighbour in the specified dimension.
     */
    void to_neighbour(const T dim, const PosNeg direction){
        for(T i = 0; i < m_nCorners; ++i){
            m_corners[i][dim]+=static_cast<T>(direction);
        }
    }

    /**
     * Find all of the subboxes after binary subdivision.
     */
    box<N,T> subbox(const T ind) const{
        return box<N,T>(m_dimensions, m_level + 1, ind);
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
                innerPoints[k] = m_corners[0];
                innerPoints[k].set_level(m_level-1);
                innerPoints[k] += index<N,T>(ternary, m_level-1);
                k++;
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