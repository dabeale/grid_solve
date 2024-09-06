// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef LIB_MATH_TENSOR_HPP_
#define LIB_MATH_TENSOR_HPP_

#include "math/vector.hpp"
#include "base/dimensions.hpp"

namespace gs {
template<typename T, size_t... Dims>
/**
 * \brief A stack allocated tensor.
 * 
 * The stack allocated tensor is a tensor of arbitrary
 * dimension. It inherits from a vector, and so it shares 
 * all of the basic operations such as +,-,* etc. 
 * 
 * The elements are obtained using the call operator, with 
 * variadic inputs.
 * 
 * For example, a 3D tensor could be allocated as follows,
 * tensor<double, 3,5,7> tens;
 * 
 * The elements would then be obtained using, for example,
 * auto val = tens(0,1,5);
 * 
 * The template parameters,
 *      T       - The base type (e.g. double or float).
 *      Dims... - The dimensions.
 */
class tensor: public gs::vector<T, mult<Dims...>()> {
protected:
    static constexpr size_t m_nSize = sizeof...(Dims);
    static constexpr size_t m_nElems = mult<Dims...>();
    dimensions<m_nSize, size_t> m_dims;
public:
    tensor(): gs::vector<T, m_nElems>(), m_dims({Dims...}, 0) {}
    tensor(const gs::vector<T, m_nElems>& vec):
        gs::vector<T, m_nElems>(vec), m_dims({Dims...}, 0) {}
    tensor(std::initializer_list<T> inList):
        gs::vector<T, m_nElems>(inList), m_dims({Dims...}, 0) {}
    /**
     * \brief Get the dimensions of the tensor.
     */
    const dimensions<m_nSize, size_t>& get_dims() const {return m_dims;}
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    /**
     * \brief Return the value of the tensor at the index specified
     * by inds. The input is a variadic parameter pack.
     */
    T& operator()(Indices... inds) {
        return gs::vector<T, m_nElems>::operator()(m_dims.sub2ind({inds...}));
    }
    template<typename... Indices>
    requires (sizeof...(Indices) == m_nSize)
    /**
     * \brief Return the value of the tensor at the index specified
     * by inds. The input is a variadic parameter pack.
     */
    const T& operator()(Indices... inds) const {
        return gs::vector<T, m_nElems>::operator()(m_dims.sub2ind({inds...}));
    }
};  // NOLINT(readability/braces)
}  // namespace gs

#endif  // LIB_MATH_TENSOR_HPP_
