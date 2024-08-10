
#ifndef _GS_DIMENSIONS_
#define _GS_DIMENSIONS_

#include <iostream>
#include <inttypes.h>
#include <array>

#include "base/concepts.hpp"

namespace gs {
template<int N, typename T=uint32_t>
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
 * The template parameters,
 *      N - The number of dimensions of the box
 *      T - The integral type.
 */
class dimensions {
    std::array<T, N> m_dimensions; ///< The base array storage for the dimensions.
    T m_maxLevel; ///< The maximum allowed level.

public:
    dimensions(): m_dimensions{}, m_maxLevel(0) {m_dimensions.fill(0);}
    dimensions(std::array<T, N> dimensions, T maxLevel):  m_dimensions(dimensions), m_maxLevel(maxLevel) {}
    dimensions(T dimensions, T maxLevel): m_dimensions{}, m_maxLevel(maxLevel) {
        m_dimensions.fill(dimensions);
    }

    /**
     * \brief Get the dimensions at the specified level.
     */
    std::array<T, N> level_dims(const T level) const {
        std::array<T, N> levelDims;
        for (T i=0; i<N; ++i){
            levelDims[i] = m_dimensions[i]*(1 << level);
        }
        return levelDims;
    }

    /**
     * \brief Return a dimensions object which is reduced by
     * one in every dimension.
     * 
     * Using this method allows easy conversion between indices for
     * boxes and indices for points. In the former case there is
     * one fewer coordinate in each dimension. For example, 
     * a 3 by 3 grid, has 2 by 2 boxes.
     */
    dimensions<N,T> reduce() const {
        dimensions<N,T> ret(*this);
        for (T i=0; i<N; ++i){
            ret.m_dimensions[i] = (m_dimensions[i] > 1) ? (m_dimensions[i] - 1) : 1;
        }
        return ret;
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
    T max_ind(const T level) const {
        const std::array<T, N> levelDims = dimensions<N,T>::level_dims(level);
        T total = 1;
        for (const auto dim : levelDims){
            total *= dim;
        }
        return total;
    }

    /**
     * \brief Get the grid dimensions from an index, at the specified level.
     */
    std::array<T, N> ind2sub(const T ind, const T level=0) const {
        T coef = 1;
        auto levelDims = dimensions<N,T>::level_dims(level);
        // Get the index at the specified level.
        std::array<T, N> indices;
        for (T i = 1; i <= N; ++i){
            indices[N-i] = (ind / coef) % levelDims[N-i];
            coef *= levelDims[N-i];
        }
        return indices;
    }

    /**
     * \brief Get a one dimensional index representation, at the specified level.
     */
    T sub2ind(const std::array<T, N>& indices, const T level=0) const {
        const auto levelDims = dimensions<N,T>::level_dims(level);
        T retInd = indices[N-1];
        T coef = levelDims[N-1];
        for (T i=2; i<=N; ++i){
            retInd += coef*indices[N-i];
            coef *= levelDims[N-i];
        }
        return retInd;
    }
};
}

#endif