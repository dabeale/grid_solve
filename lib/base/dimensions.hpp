// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_DIMENSIONS_HPP_
#define LIB_BASE_DIMENSIONS_HPP_

#include <inttypes.h>

#include <iostream>
#include <array>
#include <exception>

#include "base/tools.hpp"
#include "base/concepts.hpp"

namespace gs {
template<int N, typename T = uint32_t>
requires std::is_integral<T>::value && (N > 0)
/**
 * \brief Dimensions for an N-dimensional hypercube.
 * 
 * The dimensions object specifies the size in each dimension
 * of a hypercube type object (tensor or grid). It can be of
 * arbitrary dimension itself, and provides a means to convert
 * between an index (coordinate) and an integral type (int). This
 * makes it easy to enumerate all the boxes and indexes at a particular
 * level on a 2^n tree.
 * 
 * The dimensions object is initialised with the dimensions at the
 * lowest level of the tree, with a maximum level. For example, dimensions
 * {1,2} specify a rectangle of length 2 and width 1. At level 2, the dimensions
 * are {2,4} and so on. 
 * 
 * Nota Bene: The number of boxes doubles on each iteration, and there 
 * are 1 more points in each dimension than boxes. The dimensions class
 * is able to index both boxes and points, using the point parameter
 * on each method. If it is true then it uses grid locations.
 * 
 * The template parameters,
 *      N - The number of dimensions of the box
 *      T - The integral type.
 */
class dimensions {
    std::array<T, N> m_dimensions;  ///< The base array storage for the dimensions.
    T m_maxLevel;  ///< The maximum allowed level.

 public:
    /**
     * \brief Return a vector in which each element is no greater than
     * size, to indicate the direction of the numeric index.
     */
    static std::array<T, N> unitary(T ind, T size = 1) {
        ind %= pow<2, N>();
        std::array<T, N> unit;
        unit.fill(0);
        T coef = 1;
        for ( T i = 1; i <= N; ++i ) {
            unit[N-i] = ((ind / coef) % 2)*size;
            coef *= 2;
        }
        return unit;
    }

    dimensions(): m_dimensions{}, m_maxLevel(0) {m_dimensions.fill(0);}
    dimensions(std::array<T, N> dimensions, T maxLevel):
        m_dimensions(dimensions), m_maxLevel(maxLevel) {}
    dimensions(T dimensions, T maxLevel): m_dimensions{}, m_maxLevel(maxLevel) {
        m_dimensions.fill(dimensions);
    }

    /**
     * \brief The subdivision type.
     * 
     * There are currently two types of subdivision into the grid. POINTS_SUBDIVISION
     * starts with a collection of corners and recursively adds edges at each incremental
     * level of the grid. The boxes are the enclosures of the points, and so each box
     * has a non-empty intersection in terms of the points in the grid. The
     * BOXES_SUBDIVISION ensures that each box does not overlap it's neighbours.
     * 
     * Examples are given below for a 2D grid, points are represented by . in general
     * but the first box is represented by x.
     * 
     *            Level 0             Level 1              Level 2
     *        
     *         x           x       x     x     .       x  x  .  .  .
     *                                                 x  x  .  .  .
     * Points                      x     x     .       .  .  .  .  .
     *                                                 .  .  .  .  .
     *         x           x       .     .     .       .  .  .  .  .
     * 
     * 
     *         x              x     x      x.      .    x  x.  ..  ..  .
     *                                                  x  x.  ..  ..  .
     *                                                  .  ..  ..  ..  .
     * Boxes                        x      x.      .    .  ..  ..  ..  .
     *                              .      ..      .    .  ..  ..  ..  .
     *                                                  .  ..  ..  ..  .
     *                                                  .  ..  ..  ..  .
     *         x              x     .      ..      .    .  ..  ..  ..  .
     */
    enum subdivision_type {
        POINTS_SUBDIVISION = 0,  // Subdivide the points at each level
        BOXES_SUBDIVISION        // Subdivide the boxes at each level
    };

    /**
     * \brief The modality of the subdivision.
     * 
     * In either subdivision type we can specify a box or a point. The 
     * modality defines the type of output that is required.
     */
    enum modality {
        BOXES_MODE = 0,   // The index of the boxes
        POINTS_MODE,      // The index of the points
        LOCAL_BOXES,      // Local boxes as input / output
    };

    /**
     * \brief How to convert the output.
     * 
     * Conversions can be made between modalities by considering the
     * first corner point in each box. There is a one-to-one
     * mapping between the first corner point and each box.
     */
    enum conversion {
        NO_CONV = 0,  // Do not convert the point system
        POINTS_CONV,  // Convert to points mode
        BOXES_CONV,  // Convert to boxes mode
        LOCAL_CONV  // Convert to local boxes mode
    };

    /**
     * \brief Get the dimensions at the specified level.
     * 
     * The dimensions at the ith level form a difference equation,
     * $d_{i+1} = 2d_i - 1$, which can be solved since it is linear.
     * 
     * An exception is made when the dimensions equal one, since the
     * difference equation does not hold. The dimension becomes 2 at the
     * first level in this case.
     */
    std::array<T, N> level_dims(
        const T level,
        const subdivision_type subDiv,
        const modality mode
    ) const {
        std::array<T, N> levelDims;
        levelDims.fill(0);
        switch ( mode ) {
            case dimensions<N, T>::POINTS_MODE:
                switch ( subDiv ) {
                    case dimensions<N, T>::POINTS_SUBDIVISION:
                        // The dimensions for the point grid
                        for ( T i = 0; i < N; ++i ) {
                            T dimension = m_dimensions[i];
                            T levelToUse = level;
                            if ( m_dimensions[i] == 1 ) {
                                switch ( level ) {
                                    case 0:
                                        break;
                                    case 1:
                                        // Make sure that the new dimension is 2
                                        dimension = 2;
                                        levelToUse = 0;
                                        break;
                                    default:
                                        // Revert to normal behaviour.
                                        dimension = 2;
                                        levelToUse = level-1;
                                }
                            }
                            levelDims[i] = ((dimension-1) << levelToUse) + 1;
                        }
                        break;
                case dimensions<N, T>::BOXES_SUBDIVISION:
                    for ( T i = 0; i < N; ++i ) {
                        levelDims[i] = m_dimensions[i] << level;
                    }
                    break;
                }
                break;
            case dimensions<N, T>::BOXES_MODE:
                switch ( subDiv ) {
                    case dimensions<N, T>::POINTS_SUBDIVISION:
                        // The dimensions of each of the boxes - 1
                        // less in each dimension since the dimensions
                        // are specified in terms of the points.
                        for ( T i = 0; i < N; ++i ) {
                            levelDims[i] = (m_dimensions[i]-1) << level;
                        }
                        break;
                    case dimensions<N, T>::BOXES_SUBDIVISION:
                        // There are a factor of half less boxes than points in
                        // box subdivision.
                        for ( T i = 0; i < N; ++i ) {
                            levelDims[i] = (
                                (level > 0) ?
                                m_dimensions[i] << (level-1) :
                                m_dimensions[i] >> 1
                            );
                        }
                        break;
                }
                break;
            case dimensions<N, T>::LOCAL_BOXES:
                // In each case the dimensions are 2 in every direction
                // apart from at the base level, which are specified by
                // dimensions themselves.
                for ( T i = 0; i < N; ++i ) {
                    levelDims[i] = (
                        (level > 0) ? 2 : m_dimensions[i]
                    );
                }
                break;
        }
        return levelDims;
    }

    /**
     * \brief Get the maximum level.
     */
    T max_level() const {
        return m_maxLevel;
    }

    /**
     * \brief Get the maximum index at the specified level.
     */
    T max_ind(
        const T level,
        const subdivision_type subDiv,
        const modality mode
    ) const {
        const std::array<T, N> levelDims = dimensions<N, T>::level_dims(
            level, subDiv, mode
        );
        T total = 1;
        for ( const auto dim : levelDims ) {
            total *= dim;
        }
        return total;
    }

    /**
     * \brief Get the grid dimensions from an index, at the specified level.
     */
    std::array<T, N> ind2sub(
        const T ind,
        const T level = 0,
        const subdivision_type subDiv = POINTS_SUBDIVISION,
        const modality mode = POINTS_MODE,
        const conversion conv = dimensions<N, T>::NO_CONV
    ) const {
        T coef = 1;
        std::array<T, N> levelDims = dimensions<N, T>::level_dims(level, subDiv, mode);
        // Get the index at the specified level.
        std::array<T, N> indices;
        for ( T i = 1; i <= N; ++i ) {
            indices[N-i] = (ind / coef) % levelDims[N-i];
            switch (conv) {
            case dimensions<N, T>::POINTS_CONV:
                if (subDiv == dimensions<N, T>::BOXES_SUBDIVISION) indices[N-i]*=2;
                break;
            case dimensions<N, T>::BOXES_CONV:
                if (subDiv == dimensions<N, T>::BOXES_SUBDIVISION) indices[N-i]/=2;
                break;
            case dimensions<N, T>::LOCAL_CONV:
                indices[N-i]%=2;
                break;
            case dimensions<N, T>::NO_CONV:
                break;
            }
            coef *= levelDims[N-i];
        }
        return indices;
    }

    /**
     * \brief Get a one dimensional index representation, at the specified level.
     */
    T sub2ind(
        std::array<T, N> indices,
        const T level = 0,
        const subdivision_type subDiv = POINTS_SUBDIVISION,
        const modality mode = POINTS_MODE,
        const conversion conv = dimensions<N, T>::NO_CONV
    ) const {
        std::array<T, N> levelDims;

        switch ( conv ) {
        case dimensions<N, T>::POINTS_CONV:
            levelDims = dimensions<N, T>::level_dims(level, subDiv, dimensions<N, T>::POINTS_MODE);
            break;
        case dimensions<N, T>::BOXES_CONV:
            levelDims = dimensions<N, T>::level_dims(level, subDiv, dimensions<N, T>::BOXES_MODE);
            break;
        case dimensions<N, T>::LOCAL_CONV:
            levelDims = dimensions<N, T>::level_dims(level, subDiv, dimensions<N, T>::LOCAL_BOXES);
            break;
        case dimensions<N, T>::NO_CONV:
            levelDims = dimensions<N, T>::level_dims(level, subDiv, mode);
            break;
        }

        T coef = levelDims[N-1];

        switch ( conv ) {
        case dimensions<N, T>::POINTS_CONV:
            if (subDiv == dimensions<N, T>::BOXES_SUBDIVISION) {
                for ( auto& ind : indices ) ind <<= 1;
            }
            break;
        case dimensions<N, T>::BOXES_CONV:
            if (subDiv == dimensions<N, T>::BOXES_SUBDIVISION) {
                for ( auto& ind : indices ) ind >>= 1;
            }
            break;
        case dimensions<N, T>::LOCAL_CONV:
            for ( T i = 0; i < N; ++i ) indices[i] %= levelDims[i];
        case dimensions<N, T>::NO_CONV:
            break;
        }

        T retInd = indices[N-1];
        for ( T i = 2; i <= N; ++i ) {
            DEBUG_ASSERT(indices[N-i] < levelDims[N-i])
            retInd += coef*indices[N-i];
            coef *= levelDims[N-i];
        }
        return retInd;
    }
};
}  // namespace gs

#endif  // LIB_BASE_DIMENSIONS_HPP_
