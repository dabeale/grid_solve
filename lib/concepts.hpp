
#ifndef _GS_CONCEPTS_
#define _GS_CONCEPTS_

namespace gs {
/**
 * Random access container concept
 */
template<typename T>
concept random_access_container = requires(T t) {
    t[int()];
};
/**
 * Iterable concept
 */
template<typename T>
concept iterable = requires(T t) {
    t.begin();
    t.end();
};
}

#endif