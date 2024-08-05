
#include <iostream>
#include "base/tools.hpp"
#include "math/matrix.hpp"
#include "math/tensor.hpp"
#include "math/equi_tensor.hpp"
#include "math/polynomial.hpp"
#include "math/taylor.hpp"
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

int test_polynomial(){
    std::cout << "Test polynomial" << std::endl;
    int retVal = 0;
    {
        polynomial<double, 2, 3> mat;
        std::array<vector<double,3>, 1> inVec{
            vector<double,3>({1,2,3})
        };
        mat.fill(inVec);
        auto eval = mat.evaluate(vector<double,3>({6,2,1}));
        retVal += assert_bool(std::abs(eval - 183) < 1e-8, "std::abs(eval - 183) < 1e-8");
    }
    {
        polynomial<double, 3, 2> mat;
        std::array<vector<double,2>, 1> inVec{
            vector<double,2>({1,2})
        };
        mat.fill(inVec);
        auto eval = mat.evaluate(vector<double,2>({1,0}));
        retVal += assert_bool(std::abs(eval - 4) < 1e-8, "std::abs(eval - 4) < 1e-8");
    }
    {
        polynomial<double, 3, 2> mat;
        std::array<vector<double,2>, 1> inVec{
            vector<double,2>({1,2})
        };
        mat.fill(inVec);
        auto eval = mat.evaluate(vector<double,2>({0,1}));
        retVal += assert_bool(std::abs(eval - 15) < 1e-8, "std::abs(eval - 15) < 1e-8");
    }
    return retVal;
}

int test_taylor(){
    std::cout << "Test taylor" << std::endl;
    int retVal = 0;
    {
        taylor<double, 3, 5, exp_squared> tlor{
            exp_squared<double, 3, 5>()
        };
        exp_squared<double, 3> comp;
        for(double pert : {0.1, 0.2, -0.1, 0.001}){
            {
                vector<double, 3> val({1+pert,2,3});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += assert_bool(std::abs(expected - estimate) < 1e-6, "std::abs(expected - estimate) < 1e-6");
            }
            {
                vector<double, 3> val({1,2+pert,3});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += assert_bool(std::abs(expected - estimate) < 1e-6, "std::abs(expected - estimate) < 1e-6");
            }
            {
                vector<double, 3> val({1,2,3+pert});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += assert_bool(std::abs(expected - estimate) < 1e-6, "std::abs(expected - estimate) < 1e-6");
            }
        }
        {
            taylor<double, 3, 2, exp_squared> tlora{ exp_squared<double, 3, 2>() };
            taylor<double, 3, 5, exp_squared> tlorb{ exp_squared<double, 3, 5>() };
            taylor<double, 3, 10, exp_squared> tlorc{ exp_squared<double, 3, 10>() };
            exp_squared<double, 3> comp;
            for(double pert : {0.1, 0.2, -0.1, 0.001}){
                {
                    vector<double, 3> val({1+pert,2,3});
                    auto expected = comp(val, {2,2,3});
                    auto estimatea = tlora.estimate(val, {1,2,3}, {2,2,3});
                    auto estimateb = tlorb.estimate(val, {1,2,3}, {2,2,3});
                    auto estimatec = tlorc.estimate(val, {1,2,3}, {2,2,3});
                    retVal += assert_bool(
                        std::abs(expected - estimatea) < 1e-2,
                        "std::abs(expected - estimatea) < 1e-2"
                    );
                    retVal += assert_bool(
                        std::abs(expected - estimateb) < std::abs(expected - estimatea),
                        "std::abs(expected - estimateb) < std::abs(expected - estimatea)"
                    );
                    // Use <= because beyond a certain precision they are the same.
                    retVal += assert_bool(
                        std::abs(expected - estimatec) <= std::abs(expected - estimateb),
                        "std::abs(expected - estimatec) <= std::abs(expected - estimateb)"
                    );
                }
            }
        }
    }
    return retVal;
}