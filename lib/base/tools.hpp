
#ifndef _GS_TOOLS_
#define _GS_TOOLS_

#include <iostream>
#include <string>

#define ASSERT_BOOL(value) gs::assert_bool(value, #value);

namespace gs {
/**
 * \brief Print a message if the predicate is false.
 */
bool assert_bool(bool val, std::string errMessage){
    if ( ! val ){
        std::cout << errMessage << std::endl;
    }
    return ! val;
}

template<size_t Base, size_t Exponent>
/**
 * \brief Compute power using the pre-compiler.
 */
constexpr size_t pow(){
    if constexpr (Exponent == 0){
        return 1;
    }
    else {
        return pow<Base, Exponent-1>()*Base;
    }
}

template<size_t Val, size_t... Vals>
/**
 * \brief Compute multiplication using the pre-compiler.
 */
constexpr size_t mult(){
    if constexpr (sizeof...(Vals) == 0){
        return Val;
    }
    else {
        return mult<Vals...>()*Val;
    }
}

template<typename T, size_t N, typename... Args>
/**
 * \brief Remove the specified indices from an array.
 */
std::array<T, N-sizeof...(Args)> remove_i(const std::array<T, N>& ain, Args... args ){
    std::array<T, N-sizeof...(Args)> ret;
    ret.fill(0);
    size_t ind = 0;
    for(size_t i=0; i<N && ind < N-sizeof...(Args); ++i){
        bool bAdd = true;
        ([&]{bAdd &= (i!=args);}(), ...);
        if(bAdd){
            ret[ind++] = ain[i];
        }
    }
    return ret;
}

template<size_t N>
/**
 * \brief Compute the factorial using the pre-compiler.
 */
size_t factorial(){
    if constexpr (N==0 || N==1){
        return 1;
    }
    else {
        return factorial<N-1>()*N;
    }
}
}

#endif