
#ifndef _GS_TOOLS_
#define _GS_TOOLS_

#include <string>

namespace gs {
/**
 * Print a message if the predicate is false.
 */
bool assert_bool(bool val, std::string errMessage){
    if ( ! val ){
        std::cout << errMessage << std::endl;
    }
    return ! val;
}
/**
 * Compute power using the pre-compiler.
 */
template<int Base, int Exponent>
constexpr int pow(){
    if constexpr (Exponent == 0){
        return 1;
    }
    else {
        return pow<Base, Exponent-1>()*Base;
    }
}
}

#endif