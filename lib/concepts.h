
namespace gs {
/**
 * Random access container concept
 */
template<typename T>
concept RandomAccessContainer = requires(T t) {
    t[int()];
};
/**
 * Iterable concept
 */
template<typename T>
concept Iterable = requires(T t) {
    t.begin();
    t.end();
};
}