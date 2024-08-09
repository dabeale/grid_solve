
#include <iostream>
#include <random>
#include "base/tools.hpp"
#include "functions/exp_squared.hpp"
#include "functions/exp_inner.hpp"
#include "math/vector.hpp"
#include "math/taylor.hpp"
#include "math/polynomial.hpp"

int test_taylor_estimation(){
    std::cout << "Test taylor estimation" << std::endl;
    int retVal = 0;
    {
        gs::taylor<double, 3, 5, gs::exp_squared> tlor{
            gs::exp_squared<double, 3, 5>()
        };
        gs::exp_squared<double, 3> comp;
        for(double pert : {0.1, 0.2, -0.1, 0.001}){
            {
                gs::vector<double, 3> val({1+pert,2,3});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += ASSERT_BOOL(std::abs(expected - estimate) < 1e-6);
            }
            {
                gs::vector<double, 3> val({1,2+pert,3});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += ASSERT_BOOL(std::abs(expected - estimate) < 1e-6);
            }
            {
                gs::vector<double, 3> val({1,2,3+pert});
                auto expected = comp(val, {2,2,3});
                auto estimate = tlor.estimate(val, {1,2,3}, {2,2,3});
                retVal += ASSERT_BOOL(std::abs(expected - estimate) < 1e-6);
            }
        }
        {
            gs::taylor<double, 3, 2, gs::exp_squared> tlora{ gs::exp_squared<double, 3, 2>() };
            gs::taylor<double, 3, 5, gs::exp_squared> tlorb{ gs::exp_squared<double, 3, 5>() };
            gs::taylor<double, 3, 10, gs::exp_squared> tlorc{ gs::exp_squared<double, 3, 10>() };
            gs::exp_squared<double, 3> comp;
            for(double pert : {0.1, 0.2, -0.1, 0.001}){
                {
                    gs::vector<double, 3> val({1+pert,2,3});
                    auto expected = comp(val, {2,2,3});
                    auto estimatea = tlora.estimate(val, {1,2,3}, {2,2,3});
                    auto estimateb = tlorb.estimate(val, {1,2,3}, {2,2,3});
                    auto estimatec = tlorc.estimate(val, {1,2,3}, {2,2,3});
                    retVal += ASSERT_BOOL(std::abs(expected - estimatea) < 1e-2);
                    if (std::abs(estimatea-estimateb) > 1e-12){
                        retVal += ASSERT_BOOL(std::abs(expected - estimateb) < std::abs(expected - estimatea));
                    }
                    if (std::abs(estimatec-estimateb) > 1e-12){
                        retVal += ASSERT_BOOL(std::abs(expected - estimatec) < std::abs(expected - estimateb));
                    }
                }
            }
        }
    }
    return retVal;
}


int test_taylor_coefficients_squared(){
    std::cout << "Test taylor coefficients exp squared" << std::endl;
    int retVal = 0;
    // Seed some randomness
    std::mt19937 gen(0);
    // Set up random noise quite close to the center
    std::normal_distribution dist{0.0, 0.5};
    // Define the center to approximate against
    gs::vector<double, 3> center{0,0,0};
    for(size_t i=0; i<10; ++i)
    for(double testSigma : {1.0, 2.0, 5.0})
    {
        const size_t nDegree = 10;
        // The function to approximate
        gs::exp_squared<double, 3, nDegree> expSquared(testSigma);
        gs::exp_squared<double, 3, 0> expSquaredComp(testSigma);
        // Taylors approximation
        gs::taylor<double, 3, nDegree, gs::exp_squared> tlor(expSquared);
        // We are evaluating the weighted sum of the function
        // across a collection of vectors, compared to a
        // particular evaluation point. 
        gs::vector<double, 3ul> yEvalPoint{10,0,0}; // The evaluation point
        std::array<gs::vector<double, 3UL>, 4 > vVecs {
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)},
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)},
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)}, 
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)}
        }; // The vectors
        std::array<gs::vector<double, 3UL>, 4 > vVecsMinusCenter;
        for(size_t k=0; k<4ul; ++k){
            vVecsMinusCenter[k] = vVecs[k] - center;
        }
        std::array<double, 4> tVecs{dist(gen), dist(gen), dist(gen), dist(gen)}; // The weights
        // Create the polynomial used for evaluation.
        gs::polynomial<double, 3, nDegree> poly(vVecsMinusCenter, tVecs);
        // Create the expected value (100% accurate)
        double expected = 0.0;
        for(size_t i=0; i<4u; ++i){
            expected += expSquaredComp(vVecs[i], yEvalPoint)*tVecs[i];
        }
        // Perform the polynomial time estimation.
        auto estimate = tlor.estimate(poly, center, yEvalPoint );
        // Check the accuracy
        retVal += ASSERT_BOOL(std::abs(estimate - expected) < 1e-2);
        if(std::abs(estimate - expected) >= 1e-2){
            std::cout << testSigma<< ": " << expected << "," << estimate << std::endl;
        }

    }
    return retVal;
}



int test_taylor_coefficients_inner(){
    std::cout << "Test taylor coefficients exp inner" << std::endl;
    //
    // In this test the exp_inner function is used with Taylor to
    // approximate the exp_squared function. It is better to do it this
    // way because it performs better when the input x values and 
    // y values deviate from the center. In such an example, using Taylors
    // theorem directly creates a polynomial with a choice of center.
    // In using exp_inner, we expand exp_squared as follows,
    //                         / (x - c)^T(x-c) - ||y-c||^2 \             //
    // exp_squared(x,y) == exp | -------------------------  | * exp_squared(x,c)
    //                         \          sigma^2           /
    // The left hand term on the right hand side is approximated, and the other two
    // terms are computed separately.
    //
    int retVal = 0;
    // Seed some randomness
    std::mt19937 gen(0);
    // Set up random noise quite close to the center
    std::normal_distribution dist{0.0, 0.5};
    // Define the center to approximate against
    gs::vector<double, 3> center{0,0,0};
    for(size_t i=0; i<10; ++i)
    for(double testSigma : {0.1, 0.5, 1.0, 2.0, 5.0})
    {
        const size_t nDegree = 10;
        // The function to approximate
        gs::exp_inner<double, 3, nDegree> expInner(testSigma);
        gs::exp_squared<double, 3, 0> expSquaredComp(testSigma);
        // Taylors approximation
        gs::taylor<double, 3, nDegree, gs::exp_inner> tlor(expInner);
        // We are evaluating the weighted sum of the function
        // across a collection of vectors, compared to a
        // particular evaluation point. 
        gs::vector<double, 3ul> yEvalPoint{10,0,0}; // The evaluation point
        std::array<gs::vector<double, 3UL>, 4 > vVecs {
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)},
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)},
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)}, 
            gs::vector<double, 3UL>{dist(gen), dist(gen), dist(gen)}
        }; // The vectors
        std::array<double, 4> tVecs{
            dist(gen),
            dist(gen), 
            dist(gen),
            dist(gen)
        }; // The weights

        // Create new weights so that we can approximate exp_squared
        // using exp_inner
        std::array<gs::vector<double, 3UL>, 4 > vVecsMinusCenter;
        for(size_t k=0; k<4ul; ++k){
            vVecsMinusCenter[k] = (vVecs[k] - center);
        }
        std::array<double, 4> tVecsWeighted;
        for(size_t k=0; k<4ul; ++k){
            tVecsWeighted[k] = tVecs[k]*expSquaredComp(vVecs[k], center);
        }
        // Create the polynomial used for evaluation.
        gs::polynomial<double, 3, nDegree> poly(vVecsMinusCenter, tVecsWeighted);
        // Create the expected value (100% accurate)
        double expected = 0.0;
        for(size_t i=0; i<4u; ++i){
            expected += expSquaredComp(vVecs[i], yEvalPoint)*tVecs[i];
        }
        // Perform the polynomial time estimation.
        auto estimate = tlor.estimate(poly, center, yEvalPoint);
        // Check the accuracy
        retVal += ASSERT_BOOL(std::abs(estimate - expected) < 1e-2);
        if(std::abs(estimate - expected) >= 1e-2){
            std::cout << testSigma<< ": " << expected << "," << estimate << std::endl;
        }

    }
    return retVal;
}