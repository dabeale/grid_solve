
#ifndef _GS_BOX_STACK_ITERATOR_
#define _GS_BOX_STACK_ITERATOR_

#include <vector>
#include <compare>

#include "base/box.hpp"

namespace gs {
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value && std::is_unsigned<T>::value && (N > 0)
/**
 * \brief The stack of boxes.
 */
using box_stack = std::vector<box<N, T>>;

template<int N, typename T=uint32_t>
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
    dimensions<N,T> m_dimensions;   ///< The dimensions of the grid
    box_stack<N,T> m_stack;         ///< The stack of boxes
    std::array<T, N> m_counts;      ///< The count

    /**
     * \brief Increment the counts by one.
     * 
     * Return the first index in the array which has changed
     * its value.
     */
    bool increment_counts(){
        T firstChangedIndex = N-1;
        ++m_counts[N-1];
        for (size_t i=1; i<N; ++i){
            if( m_counts[N-i] == m_nSubBoxes){
                m_counts[N-1]=0;
                firstChangedIndex = N-i-1;
                ++m_counts[firstChangedIndex];
            }
        }
        if(m_counts[0] > m_nSubBoxes){
            m_counts[0] = m_nSubBoxes;
            firstChangedIndex = 0;
        }
        return firstChangedIndex;
    }
public:
    static constexpr size_t m_nSubBoxes = pow<2,N-1>(); ///< The number of subboxes

    box_stack_iterator(const dimensions<N,T>& dims, const bool past_end=false):
        m_dimensions(dims)
    {
        const T start_offset=0;
        m_counts.fill(0);
        if(past_end){
            m_counts[0]=m_nSubBoxes;
        } 
        else {
            m_stack.reserve(dims.max_level());
            for(size_t level=0; level<dims.max_level(); ++level){
                m_stack.push_back(
                    box<N,T>(m_dimensions, level, start_offset)
                );
            }
        }
    }

    /**
     * \brief Increment the iterator by 1 and return
     * the incremented object.
     */
    box_stack_iterator<N,T>& operator++(){
        T firstChangedIndex = increment_counts();
        if(firstChangedIndex > 0){
            for(size_t i=firstChangedIndex; i<N; ++i){
                m_stack[i] = m_stack[i-1].subbox(m_counts[i]);
            }
        }
        else {
            m_stack.clear();
        }
        return *this;
    }

    /**
     * \brief increment the iterator by 1 but return the
     * object before it was incremented.
     */
    box_stack_iterator<N,T> operator++(int){
        box_stack_iterator<N,T> boxIt(*this);
        box_stack_iterator<N,T>::operator++();
        return boxIt;
    }

    /**
     * \brief Equality for the iterator
     */
    bool operator==(const box_stack_iterator<N,T>& other) const{
        return m_counts == other.m_counts;
    }

    /**
     * \brief Partial ordering for the iterator.
     */
    std::partial_ordering operator<=>(const box_stack_iterator<N,T>& other) const {
        for(size_t i=0; i<N; ++i){
            if(m_counts[i] < other.m_counts[i]){
                return std::partial_ordering::less;
            }
            else if(m_counts[i] > other.m_counts[i]){
                return std::partial_ordering::greater;
            }
        }
        return std::partial_ordering::equivalent;
    }

    const box_stack<N,T>& operator*(){return m_stack;} ///< Access the underlying stack
    operator const box_stack<N,T>& () const {return m_stack;} ///< Access the underlying stack
};
}

#endif