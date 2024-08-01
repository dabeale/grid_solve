
#ifndef _GS_EQUI_TENSOR_
#define _GS_EQUI_TENSOR_

#include "math/tensor.hpp"

namespace gs {
/**
 * A base for the equi_tensor class.
 */
template<typename T, size_t N, size_t K, size_t ...Ks>
class equi_tensor_base : public equi_tensor_base<T, N-1, K, K, Ks... >{};

template<typename T, size_t K, size_t ...Ks>
class equi_tensor_base<T, 1, K, Ks...> {
public:
    using type = tensor<T, K, Ks...>;
};

/**
 * A tensor in which all of the dimensions are equal.
 * 
 * The benefit of using this class is that the number of
 * dimensions can be expressed using a const integer, rather
 * than a full pack.
 * 
 * For example, tensor<double, M, M, M> is the same as
 * equi_tensor<double, M, 3>.
 */
template<typename T, size_t N, size_t K>
class equi_tensor: public equi_tensor_base<T, N, K>::type {
    using base = equi_tensor_base<T, N, K>::type;
public:
    /**
     * The full inner product for the tensor.
     */
    T inner(const vector<T,K>& vec){
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
}

#endif