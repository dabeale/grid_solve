
#ifndef _GS_TENSOR_
#define _GS_TENSOR_

#include "vector.hpp"

namespace gs {
/**
 * A stack allocated tensor.
 */
template<typename T, size_t... Dims>
class tensor: public vector<T, mult<Dims...>()> {
    static constexpr size_t m_nSize = sizeof...(Dims);
    static constexpr size_t m_nElems = mult<Dims...>();
    std::array<size_t, m_nSize> m_coefs;
    void init_coefs() {
        m_coefs = {Dims...};
        for(size_t i=1; i<m_nSize; ++i){
            m_coefs[m_nSize-i-1] *= m_coefs[m_nSize-i];
        }
    }
public:
    tensor() {
        init_coefs();
    }
    tensor(std::initializer_list<T> inList): vector<T, m_nElems>(inList) {
        init_coefs();
    }
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    T& operator()(Indices... inds){
        std::array<size_t, m_nSize> inputInds{inds...};
        T linInd = inputInds[m_nSize-1];
        for(size_t i = 0; i < m_nSize-1; ++i){
            linInd += inputInds[i]*m_coefs[i+1];
        }
        return vector<T, m_nElems>::operator()(linInd);
    }
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    const T& operator()(Indices... inds) const {
        std::array<size_t, m_nSize> inputInds{inds...};
        T linInd = inputInds[m_nSize-1];
        for(size_t i = 0; i < m_nSize-1; ++i){
            linInd += inputInds[i]*m_coefs[i+1];
        }
        return vector<T, m_nElems>::operator()(linInd);
    }
};
}

#endif