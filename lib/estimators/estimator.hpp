// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_ESTIMATORS_ESTIMATOR_HPP_
#define LIB_ESTIMATORS_ESTIMATOR_HPP_

#include <utility>
#include <tuple>
#include <vector>

#include "math/vector.hpp"
#include "math/polynomial.hpp"

namespace gs {
template<
    typename T,  // The floating point type
    size_t N,    // The number of dimensions
    size_t D,    // The degree
    template < typename, size_t, size_t > class FuncEstimator  // Function estimator
>
/**
 * \brief The estimator concept.
 * 
 * The purpose of the estimator concept is to provide an abstraction for all
 * estimator classes, which are for estimating a continuous function and 
 * all of its derivatives. The function must be in a form which is compatible
 * with the Taylor estimator.
 */
concept estimator = requires(FuncEstimator<T, N, D> t) {
    {
        t(gs::vector<T, N>(), gs::vector<T, N>())
    } -> std::same_as<T>;  // We can call the function on a pair of vectors
    {
        t.estimate(
            polynomial<T, N, D>(),
            gs::vector<T, N>(),
            gs::vector<T, N>())
    } -> std::same_as<T>;  // There is an estimate method
    {
        t.compute_coefs(
            std::array<gs::vector<T, N>, 1>(),
            gs::vector<T, N>(),
            std::array<T, 1>())
    } -> std::constructible_from<polynomial<T, N, D>>;  // There is a compute coeffs method
};  // NOLINT(readability/braces)
}  // namespace gs

#endif  // LIB_ESTIMATORS_ESTIMATOR_HPP_
