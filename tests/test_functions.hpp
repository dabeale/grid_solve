
#include <iostream>

#include "base/tools.hpp"
#include "functions/exp_squared.hpp"

using namespace gs;

int test_exp(){
    std::cout << "Test exp" << std::endl;
    int retVal = 0;
    {
        exp_squared<double, 3, 0> exp_2(2.0);
        auto eVal = exp_2(vector<double,3>({1,2,3}), vector<double,3>({2,3,4}));
        retVal += assert_bool(std::abs(eVal - 0.6872892787909722) < 1e-10, "std::abs(eVal - 0.6872892787909722) < 1e-10");
    }
    {
        exp_squared<double, 3, 1> exp_2(2.0);
        auto eVal = exp_2(vector<double,3>({1,2,3}), vector<double,3>({2,3,4}));
        for(size_t k=0; k<3; ++k){
            retVal += assert_bool(std::abs(eVal[k] - 0.17182231969774306) < 1e-10, "std::abs(eVal - 0.17182231969774306) < 1e-10");
        }
    }
    {
        exp_squared<double, 3, 2> exp_2(2.0);
        auto eVal = exp_2(vector<double,3>({1,2,3}), vector<double,3>({2,3,4}));
        for(size_t i=0; i<3; ++i){
            for(size_t j=0; j<3; ++j){
                if(i==j){
                    retVal += assert_bool(std::abs(eVal(i,j) + 0.12886667007556424) < 1e-5, "std::abs(eVal + 0.12886667007556424) < 1e-5");
                } else {
                    retVal += assert_bool(std::abs(eVal(i,j) - 0.042955579924435765) < 1e-10, "std::abs(eVal - 0.042955579924435765) < 1e-10");
                }
            }
        }
    }
    {
        exp_squared<double, 3, 3> exp_2(2.0);
        auto eVal = exp_2(vector<double,3>({1,2,3}), vector<double,3>({2,3,4}));
        for(size_t i=0; i<3; ++i){
            for(size_t j=0; j<3; ++j){
                for(size_t k=0; k<3; ++k){
                    if (i==j && i==k){
                        retVal += assert_bool(std::abs(eVal(i,j,k) + 0.118128) < 1e-5, "std::abs(eVal + 0.118128) < 1e-5");
                    } else if (i==j || i==k || k==j) {
                        retVal += assert_bool(std::abs(eVal(i,j,k) + 0.0322167) < 1e-5, "std::abs(eVal + 0.0322167) < 1e-5");
                    } else {
                        retVal += assert_bool(std::abs(eVal(i,j,k) - 0.0107389) < 1e-5, "std::abs(eVal - 0.0107389) < 1e-5");
                    }
                }
            }
        }
    }
    return retVal;
}