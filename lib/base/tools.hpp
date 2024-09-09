// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_TOOLS_HPP_
#define LIB_BASE_TOOLS_HPP_

#include <algorithm>
#include <iostream>
#include <string>

#define ASSERT_BOOL(value) gs::assert_bool(value, #value);

#ifdef _GS_DEBUG
#define DEBUG_ASSERT(value) if ( !(value) ) throw std::runtime_error(#value);
#else
#define DEBUG_ASSERT(value)
#endif

namespace gs {
/**
 * \brief Print a message if the predicate is false.
 */
bool assert_bool(bool val, std::string errMessage) {
    if ( !val ) {
        std::cout << errMessage << std::endl;
    }
    return !val;
}

template<size_t Base, size_t Exponent>
/**
 * \brief Compute power using the pre-compiler.
 */
constexpr size_t pow() {
    if constexpr (Exponent == 0) {
        return 1;
    } else {
        return pow<Base, Exponent-1>()*Base;
    }
}

template<size_t Val, size_t... Vals>
/**
 * \brief Compute multiplication using the pre-compiler.
 */
constexpr size_t mult() {
    if constexpr (sizeof...(Vals) == 0) {
        return Val;
    } else {
        return mult<Vals...>()*Val;
    }
}

template<typename T, size_t N, typename... Args>
/**
 * \brief Remove the specified indices from an array.
 */
std::array<T, N-sizeof...(Args)> remove_i(
    const std::array<T, N>& ain,
    Args... args ) {
    std::array<T, N-sizeof...(Args)> ret;
    ret.fill(0);
    size_t ind = 0;
    for ( size_t i = 0; i < N && ind < N-sizeof...(Args); ++i ) {
        bool bAdd = true;
        ([&] {bAdd &= (i != args);}(), ...);
        if ( bAdd ) {
            ret[ind++] = ain[i];
        }
    }
    return ret;
}

template<size_t N>
/**
 * \brief Compute the factorial using the pre-compiler.
 */
size_t factorial() {
    if constexpr (N == 0 || N == 1) {
        return 1;
    } else {
        return factorial<N-1>()*N;
    }
}

template<size_t N, typename T>
/**
 * \brief Add two arrays
 */
std::array<T, N> operator+(std::array<T, N> a, const std::array<T, N>& b) {
    for ( size_t i = 0; i < N; ++i ) { a[i] += b[i]; }
    return a;
}
template<size_t N, typename T>
/**
 * \brief Max of two arrays.
 */
std::array<T, N> max(std::array<T, N> a, const std::array<T, N>& b) {
    for ( size_t i = 0; i < N; ++i ) { a[i] = std::max(a[i], b[i]); }
    return a;
}
template<size_t N, typename T>
/**
 * \brief Min of two arrays.
 */
std::array<T, N> min(std::array<T, N> a, const std::array<T, N>& b) {
    for ( size_t i = 0; i < N; ++i ) { a[i] = std::min(a[i], b[i]); }
    return a;
}
}  // namespace gs

#endif  // LIB_BASE_TOOLS_HPP_
