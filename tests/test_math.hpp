
#include <iostream>
#include "base/tools.hpp"
#include "math/matrix.hpp"
#include "math/tensor.hpp"
#include "math/equi_tensor.hpp"

using namespace gs;

int test_matrix(){
    std::cout << "Test matrix" << std::endl;
    int retVal = 0;
    {
        matrix<double,3u,3u> mat1({
            1,2,3,
            4,5,6,
            7,8,9
        });
        matrix<double,3u,3u> mat2({
            1,0,0,
            1,0,0,
            1,0,0
        });
        matrix<double,3u,3u> expected({
            6,0,0,
            15,0,0,
            24,0,0
        });
        retVal += assert_bool((mat1*mat2 - expected).norm2() < 1e-8,"(mat1*mat2 - expected).norm2() < 1e-8");
    }
    return retVal;
}

int test_tensor(){
    std::cout << "Test tensor" << std::endl;
    int retVal = 0;
    {
        auto tens = equi_tensor<double, 2, 3>({
            1,2,3,
            4,5,6,
            7,8,9
        });
        matrix<double,3u,3u> expected({
            1,2,3,
            4,5,6,
            7,8,9
        });
        for(size_t i=0; i<3; ++i){
            for(size_t j=0; j<3; ++j){
                retVal += assert_bool(std::abs(tens(i,j) - expected(i,j)) < 1e-8, "std::abs(tens(i,j) - expected(i,j)) < 1e-8");
            }
        }
    }
    {
        auto tens = equi_tensor<double, 2, 3>({
            1,2,3,
            4,5,6,
            7,8,9
        });
        vector<double,3u> vec({1,2,3});
        retVal += assert_bool(std::abs(tens.inner(vec) - 228) < 1e-8, "std::abs(tens.inner(vec) - 228) < 1e-8");
    }
    return retVal;
}

int test_vector(){
    std::cout << "Test vector" << std::endl;
    int retVal = 0;
    {
        matrix<double,3u,3u> mat({
            1,2,3,
            4,5,6,
            7,8,9
        });
        vector<double, 3u> vec({
            1,0,1
        });
        vector<double, 3u> expected({
            4,10,16
        });
        retVal += assert_bool((mat*vec - expected).norm2() < 1e-8,"(mat*vec - expected).norm2() < 1e-8");
    }
    return retVal;
}
