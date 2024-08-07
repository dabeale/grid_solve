
#include <iostream>
#include "base/tools.hpp"
#include "math/matrix.hpp"
#include "math/tensor.hpp"
#include "math/equi_tensor.hpp"
#include "math/polynomial.hpp"
#include "functions/exp_squared.hpp"

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
        retVal += ASSERT_BOOL((mat1*mat2 - expected).norm2() < 1e-8);
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
                retVal += ASSERT_BOOL(std::abs(tens(i,j) - expected(i,j)) < 1e-8);
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
        retVal += ASSERT_BOOL(std::abs(tens.inner(vec) - 228) < 1e-8);
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
        retVal += ASSERT_BOOL((mat*vec - expected).norm2() < 1e-8);
    }
    return retVal;
}

int test_polynomial(){
    std::cout << "Test polynomial" << std::endl;
    int retVal = 0;
    std::array<double, 1> tVals{1.0};
    {
        polynomial<double, 3, 2> mat;
        std::array<vector<double,3>, 1> vVals{
            vector<double,3>({1,2,3})
        };
        mat.fill(vVals, tVals);
        auto eval = mat.evaluate(vector<double,3>({6,2,1}));
        retVal += ASSERT_BOOL(std::abs(eval - 183) < 1e-8);
    }
    {
        polynomial<double, 2, 3> mat;
        std::array<vector<double,2>, 1> vVals{
            vector<double,2>({1,2})
        };
        mat.fill(vVals, tVals);
        auto eval = mat.evaluate(vector<double,2>({1,0}));
        retVal += ASSERT_BOOL(std::abs(eval - 4) < 1e-8);
    }
    {
        polynomial<double, 2, 3> mat;
        std::array<vector<double,2>, 1> vVals{
            vector<double,2>({1,2})
        };
        mat.fill(vVals, tVals);
        auto eval = mat.evaluate(vector<double,2>({0,1}));
        retVal += ASSERT_BOOL(std::abs(eval - 15) < 1e-8);
    }
    return retVal;
}
