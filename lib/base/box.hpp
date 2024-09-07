// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_BOX_HPP_
#define LIB_BASE_BOX_HPP_

#include <iostream>
#include <algorithm>

#include "base/dimensions.hpp"
#include "base/index.hpp"
#include "base/tools.hpp"

namespace gs {
template<int N, typename T = uint32_t>
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
    using subdivision_type = typename dimensions<N, T>::subdivision_type;

    std::array<index<N, T>, pow<2, N>()> m_corners;  ///< The corners.
    T m_level;  ///< The box level.
    T m_indexInParent;  ///< The index of the box within it's parent.
    T m_offset;  ///< The box index (offset)
    dimensions<N, T> m_dimensions;  ///< The dimensions of the box.
    subdivision_type m_subdivType;  ///< The subdivision type

    /**
     * \brief Find the index of the box within its parent box.
     * 
     * This method is called on construction, and stored in the
     * m_indexInParent variable. It is therefore private.
     */
    T compute_index_in_parent() const {
        if ( m_level > 0 ) {
            // Convert the index vector into local box coordinate
            return m_dimensions.sub2ind(
                // Convert the offset into a local box coordinate.
                m_dimensions.ind2sub(
                    m_offset, m_level, m_subdivType, dimensions<N, T>::BOXES_TO_LOCAL_BOXES
                ),
                m_level - 1,
                m_subdivType,
                dimensions<N, T>::LOCAL_BOXES
            );
        } else {
            return 0;
        }
    }

 public:
    static constexpr T m_nCorners = pow<2, N>();  ///< The number of corners.
    static constexpr T m_nSubPoints = pow<3, N>();  ///< The number of subpoints.

    box(
        const dimensions<N, T> inDims,
        const T level,
        const subdivision_type subdivType,
        const T offset = 0,
        const T indexInParent = static_cast<T>(-1)
    ):
        m_level(level),
        m_indexInParent(indexInParent),
        m_offset(offset),
        m_dimensions(inDims),
        m_subdivType(subdivType) {
        for ( T i = 0; i < m_nCorners; ++i ) {
            m_corners[i] = (
                // Add each of the corners a max distance of 1 from the offset.
                index<N, T>(dimensions<N, T>::unitary(i), level) +
                // The start corner point
                index<N, T>(
                    m_dimensions.ind2sub(
                        offset,
                        level,
                        subdivType,
                        dimensions<N, T>::BOXES_TO_POINTS
                    ),
                    level
                )
            );
        }
        if ( indexInParent == static_cast<T>(-1) ) {
            m_indexInParent = compute_index_in_parent();
        }
    }

    T get_offset() const {return m_offset;}  ///< Get the box offset
    T get_level() const {return m_level;}  ///< Get the current level.
    T index_in_parent() const {return m_indexInParent;}  ///< Get the index of the box within it's parent.

    /**
     * \brief A neighbour direction of a box.
     */
    enum PosNeg {
        POSITIVE = 1,
        NEGATIVE = -1
    };
    /**
     * \brief Change the box to its neighbour in the specified dimension.
     */
    void to_neighbour(const T dim, const PosNeg direction) {
        for ( T i = 0; i < m_nCorners; ++i ) {
            m_corners[i][dim] += static_cast<T>(direction);
        }
    }

    /**
     * \brief Check whether the given index is inside the box.
     * 
     * Depending on the strict argument the method will check whether
     * the point is strictly inside or not. For example, if strict
     * is true and the point is on one of the corners then the method
     * will return false.
     */
    bool is_inside(index<N, T> ind, const subdivision_type subdivType, const bool strict = false) const {
        box<N, T> levelBox(*this);
        // Ensure that the index is the correct level
        if (ind.get_level() < m_level) {
            ind.set_level(m_level, subdivType);
        } else if ( ind.get_level() > m_level ) {
            for ( size_t i = 0; i < m_nCorners; ++i ) {
                levelBox.m_corners[i].set_level(
                    ind.get_level(),
                    subdivType
                );
            }
        }
        // Check whether the point is inside in each dimension
        auto maxInd = levelBox.max();
        auto minInd = levelBox.min();
        for ( size_t i = 0; i < N; ++i ) {
            if (
                ( (!strict) && (ind[i] <  minInd[i] || ind[i] >  maxInd[i]) ) ||
                (   strict  && (ind[i] <= minInd[i] || ind[i] >= maxInd[i]) )
            ) {
                return false;
            }
        }
        return true;
    }

    /**
     * \brief The parent of the box.
     * 
     * Return the parent box, if it exists. If we are at the top of the
     * tree then return self.
     */
    box<N, T> parent() const {
        if ( m_level > 0 ) {
            return box<N, T>(
                m_dimensions,
                m_level - 1,
                m_subdivType,
                m_dimensions.sub2ind(
                    m_corners[0].at_level(m_level - 1),
                    m_level - 1,
                    m_subdivType,
                    dimensions<N, T>::POINTS_TO_BOXES
                )
            );
        } else {
            return *this;
        }
    }

    /**
     * \brief A subbox after binary subdivision.
     * 
     * The input index is the index of the nth subbox. It
     * cannot be higher that the number of corners.
     */
    box<N, T> subbox(const T ind) const {
        // Find the original offset in terms of a box vector
        auto subp1 = m_dimensions.ind2sub(
            m_offset,
            m_level,
            m_subdivType,
            dimensions<N, T>::BOXES_MODE
        );
        // Move the box up a level. Note that in both subdivision methods
        // there is always a factor of two more boxes at the next level.
        for ( auto& v : subp1 ) v *= 2;
        // Return the subbox which is an offset of one in the direction
        // specified by ind
        return box<N, T>(
            m_dimensions,
            m_level + 1,
            m_subdivType,
            m_dimensions.sub2ind(
                subp1 + dimensions<N, T>::unitary(ind),
                m_level+1, m_subdivType, dimensions<N, T>::BOXES_MODE
            ),
            ind
        );
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
    T n_nbrs() const {
        return (
            m_level == 0 ?
            m_dimensions.max_ind(
                0,
                m_subdivType,
                dimensions<N, T>::BOXES_MODE
            ) : box<N, T>::m_nCorners
        );
    }

    /**
     * \brief Print the box at the specified level.
     * 
     * This method is for testing purposes, to allow one to
     * see the vertices of the box at the lowest level.
     */
    void print(const T level) const {
        for ( T i = 0; i < m_nCorners; ++i ) {
            std::cout << m_corners[i].at_level(level, m_subdivType) << " ";
        }
        std::cout << std::endl;
    }

    /**
     * \brief Find all of the subpoints after binary subdivision.
     * 
     * This does not include the corners of the box.
     */
    std::array<index<N, T>, m_nSubPoints - m_nCorners> subpoints() const {
         std::array<index<N, T>, m_nSubPoints - m_nCorners> innerPoints;
         std::array<T, N> ternary;
         T k = 0;
         for ( T i = 0; i < m_nSubPoints; ++i ) {
            T acc = 1;
            bool allEdge = true;
            for ( T j = 0; j < N; ++j ) {
                ternary[j] = (i / acc)  % 3;
                allEdge &= (ternary[j] != 1);
                acc *= 3;
            }
            if ( !allEdge && k < m_nSubPoints - m_nCorners ) {
                innerPoints[k] = m_corners[0];
                innerPoints[k].set_level(m_level+1, m_subdivType);
                innerPoints[k] += index<N, T>(ternary, m_level+1);
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
        for ( size_t i = 0; i < m_nCorners; ++i ) {
            maxArr = gs::max(
                static_cast<std::array<T, N>>(m_corners[i]),
                maxArr
            );
        }
        return maxArr;
    }

    /**
     * \brief Find the minimum indexes in each dimension.
     */
    std::array<T, N> min() const {
        std::array<T, N> minArr;
        minArr.fill(static_cast<T>(-1));
        for ( size_t i = 0; i < m_nCorners; ++i ) {
            minArr = gs::min(
                static_cast<std::array<T, N>>(m_corners[i]),
                minArr
            );
        }
        return minArr;
    }

    const auto& operator[](const T i) const {return m_corners[i];}  ///< Access the ith corner of the box
    auto begin() -> decltype(m_corners.begin()) {return m_corners.begin();}  ///< Return a begin iterator into the corners
    auto end() -> decltype(m_corners.end()) {return m_corners.end();}  ///< Return the end iterator into the corners
    auto begin() const -> decltype(m_corners.begin()) {return m_corners.begin();}  ///< Return a begin iterator into the corners
    auto end() const -> decltype(m_corners.end()) {return m_corners.end();}  ///< Return the end iterator into the corners
};

template<int N, typename T = uint32_t>
/**
 * \brief Append the box to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const box<N, T>& boxVar) {
    for ( auto corner : boxVar ) {
        os << corner << " ";
    }
    return os;
}
}  // namespace gs

#endif  // LIB_BASE_BOX_HPP_
