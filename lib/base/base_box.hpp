// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_BASE_BOX_HPP_
#define LIB_BASE_BASE_BOX_HPP_

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
 * The template parameters,
 *      N - The number of dimensions of the box
 *      T - The integral type.
 */
class base_box {
 public:
    static constexpr T m_nCorners = pow<2, N>();  ///< The number of corners.
    static constexpr T m_nSubPoints = pow<3, N>();  ///< The number of subpoints.

 protected:
    dimensions<N, T> m_dimensions;  ///< The dimensions of the box.
    std::array<index<N, T>, m_nCorners> m_corners;  ///< The corners.
    T m_level;  ///< The box level.

 public:
    base_box(
        const dimensions<N, T>& dims,
        const std::array<index<N, T>, m_nCorners> corners,
        const T level
    ) : m_dimensions(dims), m_corners(corners), m_level(level) { }
    base_box(
        const dimensions<N, T>& dims,
        const T level
    ) : m_dimensions(dims), m_level(level) { }
    base_box(
        const dimensions<N, T>& dims,
        const index<N, T>& firstCorner
    ) : m_dimensions(dims), m_level(firstCorner.get_level()) {
        for ( T i = 0; i < m_nCorners; ++i ) {
            m_corners[i] = (
                // Add each of the corners a max distance of 1 from the offset.
                index<N, T>(dimensions<N, T>::unitary(i), m_level) +
                // The start corner point
                firstCorner
            );
        }
    }

    T get_level() const {return m_level;}  ///< Get the current level.

    /**
     * \brief Change the box to its neighbour in the specified dimension.
     */
    void to_neighbour(const T dim, const T stepSize) {
        DEBUG_ASSERT(dim < N)
        for ( T i = 0; i < m_nCorners; ++i ) {
            m_corners[i][dim] += stepSize;
        }
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

    auto& operator[](const T i) {return m_corners[i];}  ///< Access the ith corner of the box
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
std::ostream& operator<<(std::ostream& os, const base_box<N, T>& boxVar) {
    for ( auto corner : boxVar ) {
        os << corner << " ";
    }
    return os;
}
}  // namespace gs

#endif  // LIB_BASE_BASE_BOX_HPP_
