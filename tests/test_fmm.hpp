
#include "algorithm/fmm.hpp"
#include "functions/exp_inner.hpp"
#include "functions/exp_squared.hpp"
#include "math/taylor.hpp"
#include "estimators/exp_squared_est.hpp"
#include "implementation/analytic_multiply.hpp"


int testq_fmm_exp2_1d(){
    std::cout << "Test fmm exp2 1d" << std::endl;
    int retVal = 0;

    // Define base types
    const size_t nDims = 1;
    const size_t nDegree = 2;

    // Set the standard deviation.
    const double sigma = 5.0;

    // Set up FMM algorithm.
    gs::analytic_multiply<double, nDims, nDegree, gs::exp_squared_est> analyticMult(
        gs::dimensions<1>({2}, 10),
        gs::exp_squared_est<double, nDims, nDegree>(sigma)
    );

    // The grid size is the size of the maximum index at the maximum level
    // At level zero there are 2 points, and so at level 10 there are 2^11.
    const size_t size = gs::pow<2,11>();
    std::vector<double> inputVec(size, 0.0);
    for(size_t i=0; i<60; ++i)
        inputVec[970+i] = 1.0;
    analyticMult.initialise(inputVec);

    // Compute the solution
    analyticMult.compute();

    // Get output
    auto output = analyticMult.output();

    retVal += ASSERT_BOOL(output[501] > 0);
    for(const auto v :analyticMult.output()){
        retVal += ASSERT_BOOL(!std::isnan(v) && !std::isinf(v) );
    }

    std::cout << "Incomplete fmm testing, assert" << std::endl;
    retVal += ASSERT_BOOL(false); // @TODO unfinished

    return retVal;
}
