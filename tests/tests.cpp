
#include "test_index.hpp"
#include "test_box.hpp"
#include "test_dimensions.hpp"
#include "test_grid.hpp"
#include "test_fmm.hpp"

int main(int args, char* argv[]){
    std::cout << argv[0] << " test suite" << std::endl;
    int error = 0;
    error += testq_fmm_exp2_1d();
    error += test_grid();
    error += test_index_call();
    error += test_index_subscript();
    error += test_dimensions_sub2ind();
    error += test_dimensions_sub2ind_inversion();
    error += test_box();
    error += test_box_subpoints();
    if (error){
        std::cout << error << " tests failed" << std::endl;
    }
    else {
        std::cout << "All tests passed" << std::endl;
    }
}