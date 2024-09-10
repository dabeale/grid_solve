// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_BOX_STACK_ITERATOR_HPP_
#define LIB_BASE_BOX_STACK_ITERATOR_HPP_

#include <compare>

#include <vector>

#include "base/box.hpp"

namespace gs {
template<int N, typename T = uint32_t>
requires std::is_integral<T>::value && std::is_unsigned<T>::value && (N > 0)
/**
 * \brief The stack of boxes.
 */
using box_stack = std::vector<box<N, T>>;

template<int N, typename T = uint32_t>
requires std::is_integral<T>::value && std::is_unsigned<T>::value && (N > 0)
/**
 * \brief An iterator over a stack of boxes.
 * 
 * The grid is a heirarchy of boxes. To avoid overuse of the
 * stack itself the boxes are stored on the heap as a vector,
 * one for each level of the heirarchy. The stack is iterated
 * by counting the number of increments at each level.
 * 
 * The box_stack_iterator works as an iterator over the whole
 * stack, and so * casts to the stack itself. It can also be
 * incremented.
 */
class box_stack_iterator {
    using subdivision_type = typename dimensions<N, T>::subdivision_type;

    dimensions<N, T> m_dimensions;  ///< The dimensions of the grid
    box_stack<N, T> m_stack;  ///< The stack of boxes
    std::vector<T> m_counts;  ///< The counts
    T m_firstBoxMax;  ///< Max index of the first box (it not a subbox)
    T m_maxLevel;  ///< The maximum level
    subdivision_type m_subDivType;  ///< The subdivision type


    /**
     * \brief Increment the counts by one.
     * 
     * Return the first index in the array which has changed
     * its value.
     */
    T increment_counts() {
        T firstChangedIndex = m_maxLevel-1;
        ++m_counts[firstChangedIndex];
        for ( size_t i = 1; i < m_maxLevel; ++i ) {
            if ( m_counts[m_maxLevel-i] >= m_nSubBoxes ) {
                m_counts[m_maxLevel-i] = 0;
                firstChangedIndex = m_maxLevel-i-1;
                ++m_counts[firstChangedIndex];
            } else {
                break;
            }
        }
        if ( m_counts[0] > m_firstBoxMax ) {
            m_counts[0] = m_firstBoxMax;
            firstChangedIndex = 0;
        }
        return firstChangedIndex;
    }

 public:
    static constexpr size_t m_nSubBoxes = pow<2, N>();  ///< The number of subboxes

    box_stack_iterator(
        const dimensions<N, T>& dims,
        const subdivision_type
        subDiv,
        const bool past_end = false):
        m_dimensions(dims),
        m_counts(dims.max_level(), 0),
        m_firstBoxMax(dims.max_ind(0, subDiv, dimensions<N, T>::BOXES_MODE)),
        m_maxLevel(dims.max_level()),
        m_subDivType(subDiv) {
        const T start_offset = 0;
        if ( past_end ) {
            m_counts[0] = m_firstBoxMax;
        } else {
            m_stack.reserve(m_maxLevel);
            for ( size_t level = 0; level < m_maxLevel; ++level ) {
                m_stack.push_back(
                    box<N, T>(
                        m_dimensions,
                        level,
                        subDiv,
                        start_offset,
                        start_offset
                    )
                );
            }
        }
    }

    /**
     * \brief Increment the iterator by 1 and return
     * the incremented object.
     */
    box_stack_iterator<N, T>& operator++() {
        T firstChangedIndex = increment_counts();
        if ( m_counts[0] < m_nSubBoxes ) {
            for ( size_t i = firstChangedIndex; i < m_maxLevel; ++i ) {
                if ( i == 0 ) {
                    m_stack[i] = box<N, T>(
                        m_dimensions,
                        0,
                        m_subDivType,
                        m_counts[0],
                        m_counts[0]
                    );
                } else {
                    m_stack[i] = m_stack[i-1].subbox(m_counts[i]);
                }
            }
        } else {
            m_stack.clear();
        }
        return *this;
    }

    /**
     * \brief increment the iterator by 1 but return the
     * object before it was incremented.
     */
    box_stack_iterator<N, T> operator++(int) {
        box_stack_iterator<N, T> self(*this);
        box_stack_iterator<N, T>::operator++();
        return self;
    }

    /**
     * \brief Equality for the iterator
     */
    bool operator==(const box_stack_iterator<N, T>& other) const {
        return m_counts == other.m_counts;
    }

    /**
     * \brief Partial ordering for the iterator.
     */
    std::partial_ordering operator<=>(
        const box_stack_iterator<N, T>& other
    ) const {
        for ( size_t i = 0; i < m_maxLevel; ++i ) {
            if ( m_counts[i] < other.m_counts[i] ) {
                return std::partial_ordering::less;
            } else if ( m_counts[i] > other.m_counts[i] ) {
                return std::partial_ordering::greater;
            }
        }
        return std::partial_ordering::equivalent;
    }

    const box_stack<N, T>& operator*() {return m_stack;}  ///< Access the underlying stack
    operator const box_stack<N, T>& () const {return m_stack;}  ///< Access the underlying stack

    template<int M, typename S>
    friend std::ostream& operator<<(std::ostream& os, const box_stack_iterator<M, S>& it);
};

template<int N, typename T = uint32_t>
/**
 * \brief Append the iterator to an output stream.
 */
std::ostream& operator<<(std::ostream& os, const box_stack_iterator<N, T>& it) {
    const size_t M = it.m_counts.size();
    os << "[";
    for ( size_t i = 0; i < M-1; ++i ) {
        os << it.m_counts[i] << ",";
    }
    os << it.m_counts[M-1] << "]";
    return os;
}
}  // namespace gs

#endif  // LIB_BASE_BOX_STACK_ITERATOR_HPP_
