
#ifndef _GS_TOOLS_
#define _GS_TOOLS_

#include <iostream>
#include <string>

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
/**
 * \brief Compute power using the pre-compiler.
 */
template<size_t Base, size_t Exponent>
constexpr size_t pow(){
    if constexpr (Exponent == 0){
        return 1;
    }
    else {
        return pow<Base, Exponent-1>()*Base;
    }
}
/**
 * \brief Compute multiplication using the pre-compiler.
 */
template<size_t Val, size_t... Vals>
constexpr size_t mult(){
    if constexpr (sizeof...(Vals) == 0){
        return Val;
    }
    else {
        return mult<Vals...>()*Val;
    }
}
/**
 * \brief Remove the specified indices from an array.
 */
template<typename T, size_t N, typename... Args>
std::array<T, N-sizeof...(Args)> remove_i(const std::array<T, N>& ain, Args... args ){
    std::array<T, N-sizeof...(Args)> ret;
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
}

#endif