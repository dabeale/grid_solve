// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef TESTS_TEST_FUNCTIONS_HPP_
#define TESTS_TEST_FUNCTIONS_HPP_

#include <iostream>

#include "base/tools.hpp"
#include "functions/exp_squared.hpp"

int test_exp() {
    std::cout << "Test exp" << std::endl;
    int retVal = 0;
    {
        gs::exp_squared<double, 3, 0> exp_2(2.0);
        auto eVal = exp_2(
            gs::vector<double, 3>({1, 2, 3}),
            gs::vector<double, 3>({2, 3, 4}));
        retVal += ASSERT_BOOL(std::abs(eVal - 0.6872892787909722) < 1e-10);
    }
    {
        gs::exp_squared<double, 3, 1> exp_2(2.0);
        auto eVal = exp_2(
            gs::vector<double, 3>({1, 2, 3}),
            gs::vector<double, 3>({2, 3, 4}));
        for ( size_t k = 0; k < 3; ++k ) {
            retVal += ASSERT_BOOL(
                std::abs(eVal[k] - 0.17182231969774306) < 1e-10);
        }
    }
    {
        gs::exp_squared<double, 3, 2> exp_2(2.0);
        auto eVal = exp_2(
            gs::vector<double, 3>({1, 2, 3}),
            gs::vector<double, 3>({2, 3, 4}));
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 3; ++j ) {
                if ( i == j ) {
                    retVal += ASSERT_BOOL(
                        std::abs(eVal(i, j) + 0.12886667007556424) < 1e-5);
                } else {
                    retVal += ASSERT_BOOL(
                        std::abs(eVal(i, j) - 0.042955579924435765) < 1e-10);
                }
            }
        }
    }
    {
        gs::exp_squared<double, 3, 3> exp_2(2.0);
        auto eVal = exp_2(
            gs::vector<double, 3>({1, 2, 3}),
            gs::vector<double, 3>({2, 3, 4}));
        for ( size_t i = 0; i < 3; ++i ) {
            for ( size_t j = 0; j < 3; ++j ) {
                for ( size_t k = 0; k < 3; ++k ) {
                    if (i == j && i == k) {
                        retVal += ASSERT_BOOL(
                            std::abs(eVal(i, j, k) + 0.118128) < 1e-5);
                    } else if (i == j || i == k || k == j) {
                        retVal += ASSERT_BOOL(
                            std::abs(eVal(i, j, k) + 0.0322167) < 1e-5);
                    } else {
                        retVal += ASSERT_BOOL(
                            std::abs(eVal(i, j, k) - 0.0107389) < 1e-5);
                    }
                }
            }
        }
    }
    return retVal;
}

#endif  // TESTS_TEST_FUNCTIONS_HPP_
