
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
 * Random access container concept
 */
template<typename T>
concept random_access_container = iterable<T> && has_value<T> && requires(T t) {
    t[int()];
};
}

#endif