// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_BOX_HPP_
#define LIB_BASE_BOX_HPP_

#include <iostream>
#include <algorithm>

#include "base/dimensions.hpp"
#include "base/index.hpp"
#include "base/tools.hpp"
#include "base/base_box.hpp"

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
class box: public base_box<N, T> {
    using subdivision_type = typename dimensions<N, T>::subdivision_type;

    T m_indexInParent;  ///< The index of the box within it's parent.
    T m_offset;  ///< The box index (offset)
    subdivision_type m_subdivType;  ///< The subdivision type

    /**
     * \brief Find the index of the box within its parent box.
     * 
     * This method is called on construction, and stored in the
     * m_indexInParent variable. It is therefore private.
     */
    T compute_index_in_parent() const {
        if ( base_box<N, T>::m_level > 0 ) {
            // Convert the index vector into local box coordinate
            return base_box<N, T>::m_dimensions.sub2ind(
                // Convert the offset into a local box coordinate.
                base_box<N, T>::m_dimensions.ind2sub(
                    m_offset,
                    base_box<N, T>::m_level,
                    m_subdivType,
                    dimensions<N, T>::BOXES_MODE,
                    dimensions<N, T>::LOCAL_CONV
                ),
                base_box<N, T>::m_level - 1,
                m_subdivType,
                dimensions<N, T>::LOCAL_BOXES
            );
        } else {
            return 0;
        }
    }

 public:
    box(
        const dimensions<N, T> dims,
        const index<N, T>& ind,
        const subdivision_type subdivType
    ) : box<N, T>(
        dims,
        ind.get_level(),
        subdivType,
        dims.sub2ind(
            static_cast<std::array<T, N>>(ind),
            ind.get_level(),
            subdivType,
            dimensions<N, T>::POINTS_MODE,
            dimensions<N, T>::BOXES_CONV
        )
    ) {}
    box(
        const dimensions<N, T> dims,
        const T level,
        const subdivision_type subdivType,
        const T offset = 0,
        const T indexInParent = static_cast<T>(-1)
    ):  base_box<N, T>(
            dims,
            // The start corner point
            index<N, T>(
                dims.ind2sub(
                    offset,
                    level,
                    subdivType,
                    dimensions<N, T>::BOXES_MODE,
                    dimensions<N, T>::POINTS_CONV
                ),
                level
            )
        ),
        m_indexInParent(indexInParent),
        m_offset(offset),
        m_subdivType(subdivType) {
        // Compute the index in parent if it is not available.
        if ( indexInParent == static_cast<T>(-1) ) {
            m_indexInParent = compute_index_in_parent();
        }
    }

    T get_offset() const {return m_offset;}  ///< Get the box offset
    T index_in_parent() const {return m_indexInParent;}  ///< Get the index of the box within it's parent.

    /**
     * \brief Check whether the given index is inside the box.
     * 
     * Depending on the strict argument the method will check whether
     * the point is strictly inside or not. For example, if strict
     * is true and the point is on one of the corners then the method
     * will return false.
     */
    bool is_inside(index<N, T> ind, const bool strict = false) const {
        box<N, T> levelBox(*this);
        // Ensure that the index is the correct level
        if (ind.get_level() < base_box<N, T>::m_level) {
            ind.set_level(base_box<N, T>::m_level, m_subdivType);
        } else if ( ind.get_level() > base_box<N, T>::m_level ) {
            for ( size_t i = 0; i < base_box<N, T>::m_nCorners; ++i ) {
                levelBox.m_corners[i].set_level(
                    ind.get_level(),
                    m_subdivType
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
     * \brief Return the neighbour of the box, given an index.
     * 
     * The index specifies the subbox of the parent box, if the box is
     * at level zero, then it is simply the index of the box at level 0.
     */
    box<N, T> neighbour(const T ind) const {
        if ( base_box<N, T>::m_level > 0 ) {
            return parent().subbox(ind);
        } else {
            return box<N, T>(
                base_box<N, T>::m_dimensions,
                base_box<N, T>::m_level,
                m_subdivType,
                ind,
                ind
            );
        }
    }

    /**
     * \brief The parent of the box.
     * 
     * Return the parent box, if it exists. If we are at the top of the
     * tree then return self.
     */
    box<N, T> parent() const {
        if ( base_box<N, T>::m_level > 0 ) {
            auto boxIndex = base_box<N, T>::m_dimensions.ind2sub(
                m_offset,
                base_box<N, T>::m_level,
                m_subdivType,
                dimensions<N, T>::BOXES_MODE
            );
            // Move the box down a level. Note that in both subdivision methods
            // there is always a factor of two more boxes at the next level.
            for ( auto& i : boxIndex ) i /= 2;
            return box<N, T>(
                base_box<N, T>::m_dimensions,
                base_box<N, T>::m_level - 1,
                m_subdivType,
                base_box<N, T>::m_dimensions.sub2ind(
                    boxIndex,
                    base_box<N, T>::m_level - 1,
                    m_subdivType,
                    dimensions<N, T>::BOXES_MODE
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
        auto subp1 = base_box<N, T>::m_dimensions.ind2sub(
            m_offset,
            base_box<N, T>::m_level,
            m_subdivType,
            dimensions<N, T>::BOXES_MODE
        );
        // Move the box up a level. Note that in both subdivision methods
        // there is always a factor of two more boxes at the next level.
        for ( auto& v : subp1 ) v *= 2;
        // Return the subbox which is an offset of one in the direction
        // specified by ind
        return box<N, T>(
            base_box<N, T>::m_dimensions,
            base_box<N, T>::m_level + 1,
            m_subdivType,
            base_box<N, T>::m_dimensions.sub2ind(
                subp1 + dimensions<N, T>::unitary(ind),
                base_box<N, T>::m_level+1,
                m_subdivType,
                dimensions<N, T>::BOXES_MODE
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
            base_box<N, T>::m_level == 0 ?
            base_box<N, T>::m_dimensions.max_ind(
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
        for ( T i = 0; i < base_box<N, T>::m_nCorners; ++i ) {
            std::cout << base_box<N, T>::m_corners[i].at_level(level, m_subdivType) << " ";
        }
        std::cout << std::endl;
    }

    static constexpr uint32_t m_nUniqueSubPoints = (base_box<N, T>::m_nSubPoints - base_box<N, T>::m_nCorners);
    /**
     * \brief Find all of the subpoints after binary subdivision.
     * 
     * This does not include the corners of the box.
     */
    std::array<index<N, T>, m_nUniqueSubPoints> subpoints() const {
         std::array<index<N, T>, m_nUniqueSubPoints> innerPoints;
         std::array<T, N> ternary;
         T k = 0;
         for ( T i = 0; i < base_box<N, T>::m_nSubPoints; ++i ) {
            T acc = 1;
            bool allEdge = true;
            for ( T j = 0; j < N; ++j ) {
                ternary[j] = (i / acc)  % 3;
                allEdge &= (ternary[j] != 1);
                acc *= 3;
            }
            if ( !allEdge && k < m_nUniqueSubPoints ) {
                innerPoints[k] = base_box<N, T>::m_corners[0];
                innerPoints[k].set_level(base_box<N, T>::m_level+1, m_subdivType);
                innerPoints[k] += index<N, T>(ternary, base_box<N, T>::m_level+1);
                k++;
            }
         }
         return innerPoints;
    }

    /**
     * \brief Return true if the boxes are equal.
     */
    bool operator==(const box<N, T>& other) const {
        const auto level = std::max(base_box<N, T>::m_level, other.m_level);
        for ( size_t i = 0; i < pow<2, N>(); ++i ) {
            const index<N, T> cornerSelf(
                base_box<N, T>::m_corners[i].at_level(level, m_subdivType)
            );
            const index<N, T> cornerOther(
                other.m_corners[i].at_level(level, m_subdivType)
            );
            for ( size_t j = 0; j < N; ++j ) {
                if ( cornerSelf[j] != cornerOther[j] ) {
                    return false;
                }
            }
        }
        return true;
    }
};
}  // namespace gs

#endif  // LIB_BASE_BOX_HPP_
