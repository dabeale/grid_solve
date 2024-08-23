
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
    using subdivision_type = typename dimensions<N,T>::subdivision_type;

    std::array<index<N, T>, pow<2,N>()> m_corners; ///< The corners.
    T m_level; ///< The box level.
    T m_offset; ///< The box index (offset)
    dimensions<N,T> m_dimensions; ///< The dimensions of the box.
    subdivision_type m_subdivType; ///< True if the box is a duel box

public:
    static constexpr T m_nCorners = pow<2,N>();   ///< The number of corners.
    static constexpr T m_nSubPoints = pow<3,N>(); ///< The number of subpoints.

    box(const dimensions<N,T> inDims, const T level, const subdivision_type subdivType, const T offset=0):
        m_level(level), m_offset(offset), m_dimensions(inDims), m_subdivType(subdivType)
    {
        for (T i = 0; i < m_nCorners; ++i){
            m_corners[i] = (
                // Add each of the corners a max distance of 1 from the offset.
                index<N,T>(dimensions<N,T>::unitary(i), level ) + 
                // The start corner point
                index<N,T>(
                    m_dimensions.ind2sub(
                        offset,
                        level,
                        subdivType,
                        dimensions<N,T>::BOXES_TO_POINTS
                    ),
                    level
                )
            );
        }
    }
    box(const dimensions<N,T> dimensions, const std::array<index<N, T>, m_nCorners> corners, const T level):
        m_corners(corners), m_level(level), m_dimensions(dimensions)
    {}

    /**
     * \brief Get the box offset
     */
    T get_offset() const{
        return m_offset;
    }

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
     * \brief Check whether the given index is (inclusively) inside the box.
     * 
     * The the index is on the edge of corners then the method will
     * true.
     */
    bool is_inside(index<N,T> ind, const subdivision_type subdivType) const{
        box<N, T> levelBox(*this);
        if(ind.get_level() < m_level){
            ind.set_level(m_level, subdivType);
        }
        else if (ind.get_level() > m_level){
            for (size_t i=0; i<m_nCorners; ++i){
                levelBox.m_corners[i].set_level(ind.get_level(), subdivType);
            }
        }
        auto maxInd = levelBox.max();
        auto minInd = levelBox.min();
        for (size_t i=0; i<N; ++i){
            if (ind[i] < minInd[i] || ind[i] > maxInd[i]){
                return false;
            }
        }
        return true;
    }

    /**
     * \brief A subbox after binary subdivision.
     * 
     * The input index is the index of the nth subbox. It
     * cannot be higher that the number of corners.
     */
    box<N,T> subbox(const T ind) const{
        // Find the original offset in terms of a box vector
        auto subp1 = m_dimensions.ind2sub(m_offset, m_level, m_subdivType, dimensions<N,T>::BOXES_MODE);
        // Move the box up a level. Note that in both subdivision methods
        // there is always a factor of two more boxes at the next level.
        for(auto& v : subp1) v *= 2;
        // Return the subbox which is an offset of one in the direction
        // specified by ind
        return box<N,T>(
            m_dimensions,
            m_level + 1,
            m_subdivType,
            m_dimensions.sub2ind(
                subp1 + dimensions<N,T>::unitary(ind),
                m_level+1, m_subdivType, dimensions<N,T>::BOXES_MODE
            )
        );
    }

    /**
     * \brief Find the neighbour box with the specified index.
     * 
     * This method enumerates all of the boxes within the parent
     * box, not all boxes at the current layer, unless we are at
     * layer 0 - for which there is no parent box. The number of
     * neighbours is thus fixed at each layer apart from layer 0.
     * To get the number of neighbours use the n_nbrs method.
     */
    box<N,T> neighbour(const T ind) const {
        if (m_level > 0){
            return box<N,T>(
                m_dimensions,
                m_level-1,
                m_subdivType,
                m_dimensions.sub2ind(
                    m_corners[0].at_level(m_level-1, m_subdivType),
                    m_level-1,
                    m_subdivType,
                    dimensions<N,T>::BOXES_MODE
                )
            ).subbox(ind);
        }
        else {
            return box<N,T>(m_dimensions, m_level, m_subdivType, ind);
        }
    }

    /**
     * \brief Return the total number of neighbours of the box.
     * 
     * This method returns the total number of neighbours within
     * the parent box - not all of the adjacent neighbours. It is
     * designed to be used in a depth first tree search, in which
     * we only care about the parent and its leaves. Each of the leaves
     * are neighbours in this scenario.
     */
    T n_nbrs() const{
        return (
            m_level == 0 ?
            m_dimensions.max_ind(0, m_subdivType, dimensions<N,T>::BOXES_MODE) : box<N,T>::m_nCorners
        );
    }

    /**
     * \brief Find the index of the box within it's
     * parent.
     */
    T index_in_parent() const{
        std::array<T,N> remainderIndex(m_corners[0].at_level(m_level, m_subdivType));
        // Find the remainder of division by 2 
        // Division gives the index in the level up, and the
        // remainder is the position within the box. If we are
        // already at the lowest level then there is no need.
        if ( m_level > 0 ) {
            for(size_t i=0; i<N; ++i){
                remainderIndex[i] %= 2;
            }
        }
        return m_dimensions.sub2ind(remainderIndex, 0, m_subdivType, dimensions<N,T>::BOXES_MODE);
    }

    /**
     * \brief Print the box at the specified level.
     * 
     * This method is for testing purposes, to allow one to
     * see the vertices of the box at the lowest level.
     */
    void print(const T level) const{
        for (T i=0; i<m_nCorners; ++i){
            std::cout << m_corners[i].at_level(level, m_subdivType) << " ";
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
                innerPoints[k].set_level(m_level+1, m_subdivType);
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