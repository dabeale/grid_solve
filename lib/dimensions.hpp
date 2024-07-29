
#ifndef _GS_DIMENSIONS_
#define _GS_DIMENSIONS_

#include <iostream>
#include <inttypes.h>
#include <array>

#include "concepts.hpp"

namespace gs {
/**
 * \brief The dimensions of the grid.
 * 
 * The dimensions must be in multiples of 2.
 */
template<int N, typename T=uint32_t>
requires std::is_integral<T>::value
class dimensions {
    std::array<T, N> m_dimensions;
    T m_maxLevel;

public:
    dimensions() {}
    dimensions(std::array<T, N> dimensions, T maxLevel): m_maxLevel(maxLevel), m_dimensions(dimensions) {}

    /**
     * Get the dimensions at the specified level.
     */
    std::array<T, N> level_dims(const T level) const {
        std::array<T, N> levelDims;
        for (T i=0; i<N; ++i){
            levelDims[i] = m_dimensions[i]*(1 << level);
        }
        return levelDims;
    }

    /**
     * Get the maximum index at the specified level.
     */
    T max_ind(const T level) const {
        const auto levelDims = dimensions<N,T>::level_dims(level);
        T total = 1;
        for (const auto& dim : levelDims){
            total *= dim;
        }
        return total;
    }

    /**
     * Get the grid dimensions from an index, at the specified level.
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
     * Get a one dimensional index representation, at the specified level.
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