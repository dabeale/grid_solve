
#include "algorithm/fmm.hpp"

int testq_fmm_exp2_1d(){
    std::cout << "Test fmm exp2 1d" << std::endl;
    int retVal = 0;

    // Set up the functions
    gs::fmm<1> fmm_i (
        gs::dimensions<1>({2}, 10),
        [](const gs::vector_field<1,double>::box_val&, gs::vector_field<1,double>::grid_val&){

        },
        [](const gs::vector_field<1,double>::point_list&, gs::vector_field<1,double>::grid_val&){

        },
        [](const gs::vector_field<1,double>::point_list&, gs::vector_field<1,double>::box_val&){

        }
    );

    const size_t size = gs::pow<2,10>();

    // The grid size must be equal to 2^10
    ASSERT_BOOL(fmm_i.grid_size() == size);

    // Initialise the grid
    for (size_t i=0; i<size; ++i){
        fmm_i[i] = std::pair<double, std::array<double, 1>>{
            0.0, // Value
            {static_cast<double>(i)} // Position
        };
    }
    fmm_i[500].first = 1.0;
    
    // Compute the solution
    //fmm_i.compute();
    return retVal;
}
