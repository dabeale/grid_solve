
#include "algorithm/fmm.hpp"

using namespace gs;

int testq_fmm_exp2_1d(){
    std::cout << "Test fmm exp2 1d" << std::endl;
    int retVal = 0;
    fmm<1> fmm_i (
        dimensions<1>({2}, 4),
        [](const vector_field<1,double>::box_val&, vector_field<1,double>::grid_val&){

        },
        [](const vector_field<1,double>::point_list&, vector_field<1,double>::grid_val&){

        },
        [](const vector_field<1,double>::point_list&, vector_field<1,double>::box_val&){

        }
    );
    return retVal;
}
