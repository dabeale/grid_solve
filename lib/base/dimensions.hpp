
#ifndef _GS_DIMENSIONS_
#define _GS_DIMENSIONS_

#include <iostream>
#include <inttypes.h>
#include <array>

#include "base/tools.hpp"
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
     * 
     * The dimensions at the ith level form a difference equation,
     * $d_{i+1} = 2d_i - 1$, which can be solved since it is linear.
     * 
     * An exception is made when the dimensions equal one, since the
     * difference equation does not hold. The dimension becomes 2 at the
     * first level in this case.
     */
    std::array<T, N> level_dims(const T level, const bool duel=false) const {
        std::array<T, N> levelDims;
        if(!duel) {
            // The dimensions for the point grid
            for (T i=0; i<N; ++i){
                T dimension = m_dimensions[i];
                T levelToUse = level;
                if (m_dimensions[i] == 1){
                    switch(level){
                        case 0:
                            break;
                        case 1:
                            // Make sure that the new dimension is 2
                            dimension=2;
                            levelToUse=0;
                            break;
                        default:
                            // Revert to normal behaviour.
                            dimension=2;
                            levelToUse=level-1;
                    }
                }
                levelDims[i] = (dimension-1)*(1 << levelToUse) + 1;
            }
        }
        else {
            // The dimensions of each of the boxes (1 less in each dimension)
            for (T i=0; i<N; ++i){
                levelDims[i] = (m_dimensions[i]-1)*( 1 << level );
            }
        }
        return levelDims;
    }

    /**
     * \brief Return a vector in which each element is no greater than
     * size, to indicate the direction of the numeric index.
     */
    std::array<T, N> unitary(T ind, T size=1) const{
        ind %= pow<2,N>();
        std::array<T, N> unit;
        unit.fill(0);
        T coef = 1;
        for (T i = 1; i <= N; ++i){
            unit[N-i] = ((ind / coef) % 2)*size;
            coef *= 2;
        }
        return unit;
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
    T max_ind(const T level, const bool duel=false) const {
        const std::array<T, N> levelDims = dimensions<N,T>::level_dims(level, duel);
        T total = 1;
        for (const auto dim : levelDims){
            total *= dim;
        }
        return total;
    }

    /**
     * \brief Get the grid dimensions from an index, at the specified level.
     */
    std::array<T, N> ind2sub(const T ind, const T level=0, const bool duel=false) const {
        T coef = 1;
        auto levelDims = dimensions<N,T>::level_dims(level, duel);
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
    T sub2ind(const std::array<T, N>& indices, const T level=0, const bool duel=false) const {
        const auto levelDims = dimensions<N,T>::level_dims(level, duel);
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