// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_BOX_DUEL_ITERATOR_HPP_
#define LIB_BASE_BOX_DUEL_ITERATOR_HPP_

#include <compare>

#include <vector>

#include "base/box.hpp"

namespace gs {
template<int N, typename T = uint32_t>
requires std::is_integral<T>::value && std::is_unsigned<T>::value && (N > 0)
/**
 * \brief An iterator over the box subdivision duel space.
 * 
 * The boxes subdivision method divides the grid boxes into a
 * factor of 2 non-intersecting sub-boxes at each subsequent level.
 * The duel box is a box which takes one point from each adjacent
 * box.
 * 
 * The template parameters,
 *      N - The number of dimensions of the box
 *      T - The integral type.
 */
class box_duel_iterator {
    dimensions<N, T> m_dimensions;  ///< The dimensions of the grid
    T m_iterationLevel;  ///< The level at which to iterate
    base_box<N, T> m_baseBox;  ///< The base box to be iterated
    std::array<T, N> m_levelDims;  ///< The level dimensions
    bool m_pastTheEnd;  ///< True if the iterator is past-the-end.

    /**
     * \brief Increment the base box.
     */
    void increment_base() {
        const T stepSize = 2;  // Always increment by 2
        for ( T iDim = 0; iDim < N; ++iDim ) {
            bool reachedEnd = false;
            // Increment each dimension by 2 unless we have gone
            // past the end of the row, in which case stop.
            for (
                T i = 0;
                i < base_box<N, T>::m_nCorners && !reachedEnd;
                ++i
            ) {
                m_baseBox[i][iDim] += stepSize;
                reachedEnd = m_baseBox[i][iDim] >= m_levelDims[iDim];
            }
            if ( iDim == N-1 && reachedEnd ) {
                // If we're at the end of the final dimension stop entirely
                m_pastTheEnd = true;
            } else if ( reachedEnd ) {
                // If we're at the end of the row start again at the next row
                index<N, T> newBaseCorner(m_baseBox[0]);
                newBaseCorner[iDim] = 1;
                m_baseBox = base_box<N, T>(m_dimensions, newBaseCorner);
            } else {
                // Don't continue if we're not at the end.
                break;
            }
        }
    }

 public:
    box_duel_iterator(const dimensions<N, T> dims, const T iterationLevel, const bool pastTheEnd = false) :
        m_dimensions(dims),
        m_iterationLevel(iterationLevel),
        m_baseBox(dims, index<N, T>(1, iterationLevel)),
        m_levelDims(dims.level_dims(
            m_iterationLevel,
            dimensions<N, T>::BOXES_SUBDIVISION,
            dimensions<N, T>::BOXES_MODE
        )),
        m_pastTheEnd(pastTheEnd) {
            // There are one fewer duel boxes in each dimension.
            for ( auto& dim : m_levelDims ) { --dim; }
        }

    /**
     * \brief Increment the iterator by 1 and return
     * the incremented object.
     */
    box_duel_iterator<N, T>& operator++() {
        increment_base();
        return *this;
    }

    /**
     * \brief increment the iterator by 1 but return the
     * object before it was incremented.
     */
    box_duel_iterator<N, T> operator++(int) {
        box_duel_iterator<N, T> self(*this);
        box_duel_iterator<N, T>::operator++();
        return self;
    }

    /**
     * \brief Equality for the iterator
     */
    bool operator==(const box_duel_iterator<N, T>& other) const {
        if ( m_pastTheEnd && other.m_pastTheEnd ) {
            return true;
        }
        for ( T i = 0; i < i < base_box<N, T>::m_nCorners; ++i ) {
            if ( m_baseBox[i] != other.m_baseBox[i] ) return false;
        }
        return true;
    }

    /**
     * \brief Partial ordering for the iterator.
     */
    std::partial_ordering operator<=>(
        const box_stack_iterator<N, T>& other
    ) const {
        // We only need to consider one corner point because
        // the iterations move the whole box by two points in
        // each dimension.
        for ( size_t iDim = N-1; iDim < N; --iDim ) {
            if ( m_baseBox[0][iDim] < other.m_baseBox[0][iDim] ) {
                return std::partial_ordering::less;
            } else if ( m_baseBox[0][iDim] > other.m_baseBox[0][iDim] ) {
                return std::partial_ordering::greater;
            }
        }
        return std::partial_ordering::equivalent;
    }

    const base_box<N, T>& operator*() const { return m_baseBox; }  ///< Return the base box

    template<int M, typename S>
    friend std::ostream& operator<<(std::ostream& os, const box_stack_iterator<M, S>& it);
};
template<int N, typename T = uint32_t>
/**
 * \brief Append the iterator to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const box_duel_iterator<N, T>& it) {
    os << it.m_baseBox << std::endl;
    return os;
}
}  // namespace gs

#endif  // LIB_BASE_BOX_DUEL_ITERATOR_HPP_
