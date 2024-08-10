
#ifndef _GS_CONCEPTS_
#define _GS_CONCEPTS_

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
};

template<typename T>
/**
 * \brief Resizable concept.
 */
concept resizable = requires(T t) {
    t.size();
    t.resize(size_t());
};

template<typename T>
/**
 * \brief Random access concept.
 */
concept random_access = requires(T t) {
    t[int()];
};

template<typename T>
/**
 * \brief Random access container concept.
 */
concept random_access_container = iterable<T> && has_value<T> && random_access<T>;
}

#endif