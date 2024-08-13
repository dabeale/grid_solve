
#ifndef _GS_BOX_
#define _GS_BOX_

#include <iostream>
#include <algorithm>

#include "base/dimensions.hpp"
#include "base/index.hpp"
#include "base/tools.hpp"

namespace gs {
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value && std::is_unsigned<T>::value
/**
 * \brief A box (hypercube) in the grid.
 * 
 * The box is an N-dimensional hypercube. Each corner contains
 * and index which can be used to access elements of a grid.
 * The indices are specified at a particular level of a 2^N tree,
 * and so the box does that also. At the lowest level it contains
 * the 2^N corners of the grid, but it can be divided by a factor 
 * of 2 in each dimension to access higher levels. 
 * 
 * The template parameters,
 *      N - The number of dimensions of the box
 *      T - The integral type.
 */
class box {
    std::array<index<N, T>, pow<2,N>()> m_corners; ///< The corners.
    T m_level; ///< The box level.
    dimensions<N,T> m_dimensions; ///< The dimensions of the box.

public:
    static constexpr T m_nCorners = pow<2,N>();   ///< The number of corners.
    static constexpr T m_nSubPoints = pow<3,N>(); ///< The number of subpoints.

    box(const dimensions<N,T> inDims, const T level, const T offset=0): m_level(level), m_dimensions(inDims) {
        // There are 1 fewer boxes than points in each dimension
        const dimensions<N,T> reducedDims = inDims.reduce();
        // Create the start corner
        const index<N,T> offsetIndex(reducedDims.ind2sub(offset, level), level);
        // Add each of the corners a distance of 1 from the offset.
        for (T i = 0; i < m_nCorners; ++i){
            m_corners[i] = index<N,T>(inDims.ind2sub(i), level) + offsetIndex;
        }
    }
    box(const dimensions<N,T> dimensions, const std::array<index<N, T>, m_nCorners> corners, const T level):
        m_corners(corners), m_level(level), m_dimensions(dimensions)
    {}

    /**
     * \brief Get the current level.
     */
    T get_level() const {
        return m_level;
    }

    /**
     * \brief A neighbour direction of a box.
     */
    enum PosNeg {
        POSITIVE=1,
        NEGATIVE=-1
    };
    /**
     * \brief Change the box to its neighbour in the specified dimension.
     */
    void to_neighbour(const T dim, const PosNeg direction){
        for(T i = 0; i < m_nCorners; ++i){
            m_corners[i][dim]+=static_cast<T>(direction);
        }
    }

    /**
     * \brief A subbox after binary subdivision.
     * 
     * The input index is the index of the nth subbox. It
     * cannot be higher that the number of corners.
     */
    box<N,T> subbox(const T ind) const{
        return box<N,T>(
            m_dimensions,
            m_level + 1,
            // Find the index of the box in coordinate system of the
            // next level up. Reduce the dimensions first to ensure
            // that they are box dimensions rather than point dimensions.
            m_dimensions.reduce().sub2ind((
                    // Find the lowest corner of the current box in 
                    // next level up coordinates
                    std::array<T, N>(m_corners[0].at_level(m_level+1)) +
                    // Add the unitary offset in each direction
                    m_dimensions.ind2sub(ind % m_nCorners)
                ),
                m_level + 1
            )
        );
    }

    /**
     * \brief Print the box at the specified level.
     * 
     * This method is for testing purposes, to allow one to
     * see the vertices of the box at the lowest level.
     */
    void print(const T level){
        for (T i=0; i<m_nCorners; ++i){
            std::cout << m_corners[i].at_level(level) << " ";
        }
        std::cout << std::endl;
    }

    /**
     * \brief Find all of the subpoints after binary subdivision.
     * 
     * This does not include the corners of the box.
     */
    std::array<index<N, T>, m_nSubPoints - m_nCorners> subpoints() const{
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
                innerPoints[k].set_level(m_level+1);
                innerPoints[k] += index<N,T>(ternary, m_level+1);
                k++;
            }
         }
         return innerPoints;
    }

    /**
     * \brief Find the maximum indexes in each dimension.
     */
    std::array<T, N> max() const {
        std::array<T, N> maxArr;
        maxArr.fill(0);
        for(size_t i=0; i<m_nCorners; ++i){
            maxArr = gs::max(static_cast<std::array<T, N>>(m_corners[i]), maxArr);
        }
        return maxArr;
    }

    /**
     * \brief Find the minimum indexes in each dimension.
     */
    std::array<T, N> min() const {
        std::array<T, N> minArr;
        minArr.fill(static_cast<T>(-1));
        for(size_t i=0; i<m_nCorners; ++i){
            minArr = gs::min(static_cast<std::array<T, N>>(m_corners[i]), minArr);
        }
        return minArr;
    }

    const auto& operator[](const T i) const{return m_corners[i];} ///< Access the ith corner of the box
    auto begin() -> decltype(m_corners.begin()){return m_corners.begin();} ///< Return a begin iterator into the corners
    auto end() -> decltype(m_corners.end()){return m_corners.end();} ///< Return the end iterator into the corners
    auto begin() const -> decltype(m_corners.begin()){return m_corners.begin();} ///< Return a begin iterator into the corners
    auto end() const -> decltype(m_corners.end()){return m_corners.end();} ///< Return the end iterator into the corners
};

template<int N, typename T=uint32_t>
/**
 * \brief Append the box to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const box<N, T>& boxVar)
{
    for (auto corner : boxVar){
        os << corner << " ";
    }
    return os;
}
}

#endif