
#ifndef _GS_CONCEPTS_
#define _GS_CONCEPTS_

#include <concepts>

namespace gs {
/**
 * \brief The container has value_type member.
 */
template<typename T>
concept has_value = T::value_type || true;
/**
 * \brief Iterable concept.
 */
template<typename T>
concept iterable = requires(T t) {
    t.begin();
    t.end();
};
/**
 * \brief Resizable concept.
 */
template<typename T>
concept resizable = requires(T t) {
    t.size();
    t.resize(size_t());
};
/**
 * \brief Random access concept.
 */
template<typename T>
concept random_access = requires(T t) {
    t[int()];
};
/**
 * \brief Random access container concept.
 */
template<typename T>
concept random_access_container = iterable<T> && has_value<T> && random_access<T>;
}

#endif