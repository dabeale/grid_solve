
#ifndef _GS_EQUI_TENSOR_
#define _GS_EQUI_TENSOR_

#include "math/tensor.hpp"

namespace gs {
/**
 * \brief A base for the equi_tensor class.
 * 
 * The base class provides a recursive inheritance, which converts
 * the specified number of dimensions into a dimension list. The
 * final type is available at the lowest level of the inheritance
 * heirarchy.
 */
template<typename T, size_t N, size_t K, size_t ...Ks>
class equi_tensor_base : public equi_tensor_base<T, N-1, K, K, Ks... >{};

template<typename T, size_t K, size_t ...Ks>
class equi_tensor_base<T, 1, K, Ks...> {
public:
    using type = tensor<T, K, Ks...>;
};

/**
 * \brief A tensor in which all of the dimensions are equal.
 * 
 * The benefit of using this class is that the number of
 * dimensions can be expressed using a const integer, rather
 * than a full pack.
 * 
 * For example, tensor<double, M, M, M> is the same as
 * equi_tensor<double, 3, M>.
 * 
 * The template parameters,
 *      T - The base type (e.g. double or float).
 *      N - The number of dimensions
 *      K - The size of each dimension.
 */
template<typename T, size_t N, size_t K>
class equi_tensor: public equi_tensor_base<T, N, K>::type {
public:
    using base = equi_tensor_base<T, N, K>::type;
    equi_tensor(): base() {}
    equi_tensor(const vector<T, pow<K,N>()>& vec): base(vec) {}
    equi_tensor(std::initializer_list<T> inList): base(inList) {}
    /**
     * \brief The full inner product for the tensor.
     * 
     * For example, if the tensor is one dimensional then the
     * inner product is the dot product. If it is a matrix A
     * then the inner product is x^T A x for input vector x.
     */
    T inner(const vector<T,K>& vec) const{
        T out = 0;
        for(size_t i=0; i<pow<K, N>(); ++i){
            auto indArr = base::m_dims.ind2sub(i);
            T mult = vector<T, base::m_nElems>::operator()(i);
            for(size_t k=0; k<N; ++k){
                mult*=vec(indArr[k]);
            }
            out += mult;
        }
        return out;
    }
};

/**
 * \brief The outer product of a vector into an equi-tensor.
 * 
 * The tensor_outer function creates a tensor using the ND outer
 * product of the input vector. Supposing that the input is a vector
 * x, then xx^T is the 2D outer product and (x_i)(x_j)(x_k) is the
 * 3D outer product, and so on.
 */
template<typename T, size_t N, size_t K>
equi_tensor<T, N, K> tensor_outer(const vector<T,K>& vec){
    equi_tensor<T, N, K> out;
    for(size_t i=0; i<pow<K, N>(); ++i){
        auto indArr = out.get_dims().ind2sub(i);
        T mult = 1.0;
        for(size_t k=0; k<N; ++k){
            mult*=vec(indArr[k]);
        }
        out[i] = mult;
    }
    return out;
}
}

#endif