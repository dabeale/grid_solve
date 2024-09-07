// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_BASE_CONCEPTS_HPP_
#define LIB_BASE_CONCEPTS_HPP_

#include <concepts>

namespace gs {
template<typename T>
/**
 * \brief The container has value_type member.
 */
concept has_value = T::value_type || true;

template<typename T>
/**
 * \brief Iterable concept.
 */
concept iterable = requires(T t) {
    t.begin();
    t.end();
};  // NOLINT(readability/braces)

template<typename T>
/**
 * \brief Resizable concept.
 */
concept resizable = requires(T t) {
    t.size();
    t.resize(size_t());
};  // NOLINT(readability/braces)

template<typename T>
/**
 * \brief Random access concept.
 */
concept random_access = requires(T t) {
    t[int()];
};  // NOLINT(readability/braces)

template<typename T>
/**
 * \brief Random access container concept.
 */
concept random_access_container = (
    iterable<T> && has_value<T> && random_access<T>
);
}  // namespace gs

#endif  // LIB_BASE_CONCEPTS_HPP_
