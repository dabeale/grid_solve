// Copyright 2024 Daniel Beale CC BY-NC-SA 4.0

#include "tests/test_index.hpp"
#include "tests/test_box.hpp"
#include "tests/test_dimensions.hpp"
#include "tests/test_grid.hpp"
#include "tests/test_fmm.hpp"
#include "tests/test_math.hpp"
#include "tests/test_functions.hpp"
#include "tests/test_taylor.hpp"
#include "tests/test_iterator.hpp"
#include "tests/test_estimators.hpp"

int main(int, char* argv[]) {
    std::cout << argv[0] << " test suite" << std::endl;
    int error = 0;
    error += testq_fmm_exp2_2d();
    error += testq_fmm_exp2_1d();
    error += test_exp_estimator();
    error += test_bsi_boxes_1D();
    error += test_bsi_points_1D();
    error += test_subbox_duel();
    error += test_dimensions_sub2ind();
    error += test_index_call_duel();
    error += test_grid();
    error += test_subbox();
    error += test_box_subpoints();
    error += test_box();
    error += test_dimensions_sub2ind_simple();
    error += test_taylor_coefficients_inner();
    error += test_taylor_coefficients_squared();
    error += test_taylor_estimation();
    error += test_exp();
    error += test_vector();
    error += test_matrix();
    error += test_tensor();
    error += test_polynomial();
    error += test_index_call();
    error += test_index_subscript();
    error += test_dimensions_sub2ind_inversion();
    if ( error ) {
        std::cout << error << " tests failed" << std::endl;
    } else {
        std::cout << "All tests passed" << std::endl;
    }
}
