
#ifndef _GS_TENSOR_
#define _GS_TENSOR_

#include "math/vector.hpp"
#include "base/dimensions.hpp"

namespace gs {
/**
 * A stack allocated tensor.
 */
template<typename T, size_t... Dims>
class tensor: public vector<T, mult<Dims...>()> {
protected:
    static constexpr size_t m_nSize = sizeof...(Dims);
    static constexpr size_t m_nElems = mult<Dims...>();
    dimensions<m_nSize, size_t> m_dims;
public:
    tensor(): m_dims({Dims...},0) {}
    tensor(std::initializer_list<T> inList): vector<T, m_nElems>(inList), m_dims({Dims...}, 0) {}
    const dimensions<m_nSize, size_t>& get_dims() const {return m_dims;}
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    T& operator()(Indices... inds){
        return vector<T, m_nElems>::operator()(
            m_dims.sub2ind({inds...})
        );
    }
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    const T& operator()(Indices... inds) const {
        return vector<T, m_nElems>::operator()(
            m_dims.sub2ind({inds...})
        );
    }
};
}

#endif