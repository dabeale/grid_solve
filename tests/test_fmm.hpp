
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
    const size_t nDegree = 15;

    // Set the standard deviation.
    const double sigma = 2.0;
    gs::dimensions<1> dims(2, 4);
    gs::exp_squared_est<double, nDims, nDegree> estimator(sigma);

    // Set up FMM algorithm.
    gs::analytic_multiply<double, nDims, nDegree, gs::exp_squared_est> analyticMult(
        gs::dimensions<1>(2, 4), estimator
    );

    const size_t size = gs::pow<2,4>();
    std::vector<double> inputVec(size, 0.0);
    inputVec[6] = inputVec[7] = inputVec[8] = inputVec[9] = 1.0;
    analyticMult.initialise(inputVec);

    // Compute the solution
    analyticMult.compute();

    // Get output
    auto output = analyticMult.output();

    for(const auto v :analyticMult.output()){
        retVal += ASSERT_BOOL(!std::isnan(v) && !std::isinf(v) );
    }

    std::vector<double> expected(size, 0.0);
    for(size_t i=0; i<size; ++i){
        for(size_t j=0; j<size; ++j){
            expected[i] += estimator(
                gs::vector<double, 1>(dims.ind2sub(i, dims.max_level()-1, gs::dimensions<1>::BOXES_SUBDIVISION)),
                gs::vector<double, 1>(dims.ind2sub(j, dims.max_level()-1, gs::dimensions<1>::BOXES_SUBDIVISION))
            )*inputVec[j];
        }
    }
    for(size_t i=0; i<expected.size(); ++i){
        retVal += ASSERT_BOOL(std::abs(expected[i] - output[i]) < 2e-4);
    }

    return retVal;
}
