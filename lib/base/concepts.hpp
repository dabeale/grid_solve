
#ifndef _GS_CONCEPTS_
#define _GS_CONCEPTS_

#include <concepts>

namespace gs {
/**
 * The container has value_type member
 */
template<typename T>
concept has_value = T::value_type || true;
/**
 * Iterable concept
 */
template<typename T>
concept iterable = requires(T t) {
    t.begin();
    t.end();
};
/**
 * Resizable concept.
 */
template<typename T>
concept resizable = requires(T t) {
    t.size();
    t.resize(size_t());
};
/**
 * Random access concept
 */
template<typename T>
concept random_access = requires(T t) {
    t[int()];
};
/**
 * Vector concept
 */
template<typename T>
concept is_vector = random_access<T> && requires(T m, T n) {
    m(int());
    { m+n } -> std::same_as<T>;
    m+double();
    m*double();
    m/double();
    m-double();
};
/**
 * Matrix concept
 */
template<typename T>
concept is_matrix = requires(T m, T n) {
    m(int(), int());
    { m+n } -> std::same_as<T>;
    { m*n } -> std::same_as<T>;
    m+double();
    m*double();
    m/double();
    m-double();
};
/**
 * Random access container concept
 */
template<typename T>
concept random_access_container = iterable<T> && has_value<T> && random_access<T>;
}

#endif