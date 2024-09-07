// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0
#ifndef TESTS_TEST_ESTIMATORS_HPP_
#define TESTS_TEST_ESTIMATORS_HPP_

#include "estimators/exp_squared_est.hpp"

int test_exp_estimator() {
    std::cout << "Test exp estimator" << std::endl;
    int retVal = 0;

    gs::exp_squared<double, 1> exp2(2.0);
    gs::exp_squared_est<double, 1, 15> expEst(2.0);

    auto poly = expEst.compute_coefs<8>(
        {
            gs::vector<double, 1>{0.0},
            gs::vector<double, 1>{1.0},
            gs::vector<double, 1>{2.0},
            gs::vector<double, 1>{3.0},
            gs::vector<double, 1>{4.0},
            gs::vector<double, 1>{5.0},
            gs::vector<double, 1>{6.0},
            gs::vector<double, 1>{7.0},
        },
        {3.5},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0}
    );

    for ( double dd = 0.1; dd <= 0.8; dd += 0.5 ) {
        retVal += ASSERT_BOOL(
            std::abs(
                expEst.estimate(poly, {3.5}, {dd}) -
                (exp2({6.0}, {dd}) + exp2({7.0}, {dd}))) < 1e-3
        )
    }

    return retVal;
}

#endif  // TESTS_TEST_ESTIMATORS_HPP_
