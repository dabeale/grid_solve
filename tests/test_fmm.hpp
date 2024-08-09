
#include "algorithm/fmm.hpp"
#include "functions/exp_inner.hpp"
#include "functions/exp_squared.hpp"
#include "math/taylor.hpp"

int testq_fmm_exp2_1d(){
    std::cout << "Test fmm exp2 1d" << std::endl;
    int retVal = 0;

    // Define base types
    const size_t nDims = 1;
    const size_t nDegree = 2;

    using grid_val = gs::vector_field<nDims,double,nDegree>::grid_val;
    using box_val = gs::vector_field<nDims,double,nDegree>::box_val;

    // Set up the dimensions
    gs::dimensions<1> dims({2}, 10);

    // Set up the functions to approximate
    const double sigma = 10.0;
    gs::exp_inner<double, nDims, nDegree> expInner(sigma);
    gs::exp_squared<double, nDims, 0> expSquaredComp(sigma);

    // Set up the far field Taylors esimtate.
    gs::taylor<double, nDims, nDegree, gs::exp_inner> tEst(expInner);

    // Set up the functions
    auto fFarField = [&](const gs::box<1>& boxImp, const box_val& boxVal, gs::grid<1, grid_val, box_val>& grid){
        // Estimate the value at the grid point using taylor
        const int32_t level = static_cast<int32_t>(boxImp.get_level());
        for( const auto& corner : boxImp){
            auto& gridVal = grid[corner];
            if(level != std::get<3>(gridVal)){
                std::get<0>(gridVal) += tEst.estimate(boxVal.first, boxVal.second, std::get<2>(gridVal));
                std::get<3>(gridVal) = static_cast<int32_t>(level);
            }
        }
        // Interpolate into the lower level
        std::array<double, 2> weights;
        for (const auto& subIndex : boxImp.subpoints()){
            double divisor(0.0);
            for(size_t i=0; i<2; ++i){
                weights[i] = 1.0/(std::get<2>(grid[boxImp[i]]) - std::get<2>(grid[subIndex])).norm();
                divisor += weights[i];
            }
            for(size_t i=0; i<2; ++i){
                weights[i] /= divisor;
            }
            double newValue = 0;
            for(size_t i=0; i<2; ++i){
                newValue += std::get<0>(grid[boxImp[i]])*weights[i];
            }
            std::get<0>(grid[subIndex]) = newValue;
        }
    };
    auto fNearField = [&expSquaredComp](const gs::box<1>& boxImp, gs::grid<1, grid_val, box_val>& grid ){
        // Compute the precise value for the near field
        const auto ptList = grid.get_corner_values(boxImp);
        for( const auto& corner : boxImp){
            auto& gridVal = grid[corner];
            for(uint8_t i=0; i<2u; ++i){
                std::get<0>(gridVal) +=  (
                    expSquaredComp(std::get<2>(ptList[i]), std::get<2>(gridVal))*
                    std::get<1>(ptList[i])
                );
            }
        }
    };
    auto fBoxWeight = [&expSquaredComp](const gs::box<1>& boxImp, box_val& boxVal, const gs::grid<1, grid_val, box_val>& grid){
        // Find the center of the box
        const auto ptList = grid.get_corner_values(boxImp);
        auto center = (std::get<2>(ptList[0]) +  std::get<2>(ptList[1]))/2.0;

        // Reorient the points
        std::array<gs::vector<double, 1>, 2> xPts{
            std::get<2>(ptList[0]) - center,
            std::get<2>(ptList[1]) - center
        };

        // The weights are the values multiplied by
        // the squared_exponent of the distance from the center.
        std::array<double, 2> ndValues {
            std::get<1>(ptList[0]) * expSquaredComp(std::get<2>(ptList[0]), center),
            std::get<1>(ptList[1]) * expSquaredComp(std::get<2>(ptList[1]), center)
        };

        // Create an approximate polynomial given the input list.
        gs::polynomial<double, 1UL, 2UL> poly;
        poly.fill(xPts, ndValues);
        boxVal.first = poly;
        boxVal.second = center;
    };

    // Set up fmm algorithm.
    gs::fmm<1> fmm_i (dims, fFarField, fNearField, fBoxWeight);

    // The grid size is the size of the maximum index at the maximum level
    // At level zero there are 2 points, and so at level 10 there are 2^11.
    const size_t size = gs::pow<2,11>();

    // The grid size must be equal to 2^11
    retVal += ASSERT_BOOL(fmm_i.grid_size() == size);

    // Initialise the grid
    for (size_t i=0; i<size; ++i){
        fmm_i[i] = std::tuple<double, double, gs::vector<double, 1>, int32_t>{
            0.0, // Input value
            0.0, // Output Value
            {static_cast<double>(i) / 10}, // Position
            -1,  // Level computed at
        };
    }
    std::get<1>(fmm_i[500])= 1.0;
    
    // Compute the solution
    fmm_i.compute();

    // Print results
    for (size_t i=0; i<size; ++i){
        retVal += ASSERT_BOOL(!std::isnan( std::get<0>(fmm_i[i])) && !std::isinf( std::get<0>(fmm_i[i])) );
    }

    std::cout << "Incomplete fmm testing, assert" << std::endl;
    retVal += ASSERT_BOOL(false); // @TODO unfinished

    return retVal;
}
