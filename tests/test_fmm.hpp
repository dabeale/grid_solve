
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
    gs::dimensions<1> dims(2, 10);
    gs::exp_squared_est<double, nDims, nDegree> estimator(sigma);

    // Set up FMM algorithm.
    gs::analytic_multiply<double, nDims, nDegree, gs::exp_squared_est> analyticMult(
        gs::dimensions<1>(2, 10), estimator
    );

    const size_t size = gs::pow<2,10>();
    std::vector<double> inputVec(size, 0.0);
    for(size_t i=0; i<10; ++i)
        inputVec[250+i] = 1.0;
    analyticMult.initialise(inputVec);

    // Compute the solution
    analyticMult.compute();

    // Get output
    auto output = analyticMult.output();

    retVal += ASSERT_BOOL(output[255] > 0);
    for(const auto v :analyticMult.output()){
        if(v > 1e-3)
            std::cout << v << ",";
        else
            std::cout << 0 << ",";
        retVal += ASSERT_BOOL(!std::isnan(v) && !std::isinf(v) );
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::vector<double> expected(size, 0.0);
    for(size_t i=0; i<size; ++i){
        for(size_t j=0; j<size; ++j){
            expected[i] += estimator(
                gs::vector<double, 1>(dims.ind2sub(i, dims.max_level()-1)),
                gs::vector<double, 1>(dims.ind2sub(j, dims.max_level()-1))
            )*inputVec[j];
        }
    }
    for(const auto v :expected){
        if(v > 1e-3)
            std::cout << v << ",";
        else
            std::cout << 0 << ",";
    }
    std::cout << std::endl;

    std::cout << "Incomplete fmm testing, assert" << std::endl;
    retVal += ASSERT_BOOL(false); // @TODO unfinished

    return retVal;
}
